/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Map.h"
#include "ScriptMgr.h"
#include "VMapFactory.h"
#include "MapInstanced.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Transport.h"
#include "InstanceScript.h"
#include "ObjectAccessor.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Group.h"
#include "LFGMgr.h"
#include "DynamicTree.h"
#include "Vehicle.h"
#include "ObjectGridLoader.h"
#include "Profiler/ProbePoint.hpp"
#include "BattlePetSpawnMgr.h"

namespace {

union u_map_magic
{
    char asChar[4];
    uint32 asUInt;
};

u_map_magic MapMagic        = { {'M','A','P','S'} };
u_map_magic MapVersionMagic = { {'v','1','.','3'} };
u_map_magic MapAreaMagic    = { {'A','R','E','A'} };
u_map_magic MapHeightMagic  = { {'M','H','G','T'} };
u_map_magic MapLiquidMagic  = { {'M','L','I','Q'} };

#define DEFAULT_GRID_EXPIRY     300
#define MAX_GRID_LOAD_TIME      50
#define MAX_CREATURE_ATTACK_RADIUS  (45.0f * sWorld->getRate(RATE_CREATURE_AGGRO))

typedef void (*GridStateUpdate)(Map &, Map::GridContainerType::iterator, uint32);

void InvalidStateUpdate(Map &, Map::GridContainerType::iterator, uint32) { }

void ActiveStateUpdate(Map &map, Map::GridContainerType::iterator itr, uint32 diff)
{
    auto &grid = itr->second;
    auto &info = grid.getGridInfo();

    // Only check grid activity every (grid_expiry/10) ms, because it's really useless to do it every cycle
    info.UpdateTimeTracker(diff);
    if (!info.getTimeTracker().Passed())
        return;

    if (!grid.GetWorldObjectCountInNGrid<Player>() && !map.ActiveObjectsNearGrid(grid))
    {
        Trinity::ObjectGridStoper worker;
        grid.VisitAllGrids(Trinity::makeGridVisitor(worker));
        grid.SetGridState(GRID_STATE_IDLE);

        TC_LOG_DEBUG("maps", "Grid[%u, %u] on map %u moved to IDLE state",
                     grid.getX(), grid.getY(), map.GetId());
    }
    else
    {
        map.ResetGridExpiry(grid, 0.1f);
    }
}

void IdleStateUpdate(Map &map, Map::GridContainerType::iterator itr, uint32)
{
    auto &grid = itr->second;

    map.ResetGridExpiry(grid);
    grid.SetGridState(GRID_STATE_REMOVAL);
    TC_LOG_DEBUG("maps", "Grid[%u, %u] on map %u moved to REMOVAL state",
                 grid.getX(), grid.getY(), map.GetId());
}

void RemovalStateUpdate(Map &map, Map::GridContainerType::iterator itr, uint32 diff)
{
    auto &grid = itr->second;
    auto &info = grid.getGridInfo();

    if (info.getUnloadLock())
        return;

    info.UpdateTimeTracker(diff);
    if (!info.getTimeTracker().Passed())
        return;

    if (!map.UnloadGrid(itr, false))
    {
        TC_LOG_DEBUG("maps", "Grid[%u, %u] for map %u differed unloading due to players or active objects nearby",
                     grid.getX(), grid.getY(), map.GetId());
        map.ResetGridExpiry(grid);
    }
}

GridStateUpdate si_GridStates[MAX_GRID_STATE] =
{
    &InvalidStateUpdate,
    &ActiveStateUpdate,
    &IdleStateUpdate,
    &RemovalStateUpdate
};

template <typename GridVisitor, typename WorldVisitor>
void VisitNearbyCellsOf(Map *map, WorldObject* obj, GridVisitor &&gridVisitor, WorldVisitor &&worldVisitor)
{
    // Check for valid position
    if (!obj->IsPositionValid())
        return;

    // Update mobs/objects in ALL visible cells around object!
    CellArea area = Cell::CalculateCellArea(obj->GetPositionX(), obj->GetPositionY(), obj->GetGridActivationRange());

    for (uint32 x = area.low_bound.x_coord; x <= area.high_bound.x_coord; ++x)
    {
        for (uint32 y = area.low_bound.y_coord; y <= area.high_bound.y_coord; ++y)
        {
            // marked cells are those that have been visited
            // don't visit the same cell twice
            uint32 cell_id = (y * TOTAL_NUMBER_OF_CELLS_PER_MAP) + x;
            if (map->isCellMarked(cell_id))
                continue;

            map->markCell(cell_id);

            Cell cell(CellCoord(x, y));
            cell.SetNoCreate();

            map->Visit(cell, std::forward<GridVisitor>(gridVisitor));
            map->Visit(cell, std::forward<WorldVisitor>(worldVisitor));
        }
    }
}

struct CorpseGridReset final
{
    void Visit(CorpseMapType &m)
    {
        while (!m.empty())
            m.back()->RemoveFromGrid();
    }

    template <typename NotInterested>
    void Visit(NotInterested &) { }
};

} // namespace

void Map::ObjectUpdater::updateCollected(uint32 diff)
{
    if (!i_objectsToUpdate.empty())
    {
        for (auto &object : i_objectsToUpdate)
            if (object->IsInWorld())
                object->Update(diff);

        i_objectsToUpdate.clear();
    }
}

Map::~Map()
{
    sBattlePetSpawnMgr->DepopulateMap(i_mapEntry->MapID);

    UnloadAll();

    while (!i_worldObjects.empty())
    {
        WorldObject* obj = *i_worldObjects.begin();
        ASSERT(obj->IsWorldObject());
        //ASSERT(obj->GetTypeId() == TYPEID_CORPSE);
        obj->RemoveFromWorld();
        obj->ResetMap();
    }

    if (!m_scriptSchedule.empty())
        sScriptMgr->DecreaseScheduledScriptCount(m_scriptSchedule.size());
}

bool Map::ExistMap(uint32 mapid, int gx, int gy)
{
    int len = sWorld->GetDataPath().length()+strlen("maps/%03u%02u%02u.map")+1;
    char* tmp = new char[len];
    snprintf(tmp, len, (char *)(sWorld->GetDataPath()+"maps/%03u%02u%02u.map").c_str(), mapid, gx, gy);

    bool ret = false;
    FILE* pf=fopen(tmp, "rb");

    if (!pf)
        TC_LOG_ERROR("maps", "Map file '%s': does not exist!", tmp);
    else
    {
        map_fileheader header;
        if (fread(&header, sizeof(header), 1, pf) == 1)
        {
            if (header.mapMagic != MapMagic.asUInt || header.versionMagic != MapVersionMagic.asUInt)
                TC_LOG_ERROR("maps", "Map file '%s' is from an incompatible clientversion. Please recreate using the mapextractor.", tmp);
            else
                ret = true;
        }
        fclose(pf);
    }
    delete [] tmp;
    return ret;
}

bool Map::ExistVMap(uint32 mapid, int gx, int gy)
{
    if (VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager())
    {
        if (vmgr->isMapLoadingEnabled())
        {
            bool exists = vmgr->existsMap((sWorld->GetDataPath()+ "vmaps").c_str(),  mapid, gx, gy);
            if (!exists)
            {
                std::string name = vmgr->getDirFileName(mapid, gx, gy);
                TC_LOG_ERROR("maps", "VMap file '%s' is missing or points to wrong version of vmap file. Redo vmaps with latest version of vmap_assembler.exe.", (sWorld->GetDataPath()+"vmaps/"+name).c_str());
                return false;
            }
        }
    }

    return true;
}

void Map::LoadVMap(int gx, int gy)
{
                                                            // x and y are swapped !!
    int vmapLoadResult = VMAP::VMapFactory::createOrGetVMapManager()->loadMap((sWorld->GetDataPath()+ "vmaps").c_str(),  GetId(), gx, gy);
    switch (vmapLoadResult)
    {
        case VMAP::VMAP_LOAD_RESULT_OK:
            TC_LOG_INFO("maps", "VMAP loaded name:%s, id:%d, x:%d, y:%d (vmap rep.: x:%d, y:%d)", GetMapName(), GetId(), gx, gy, gx, gy);
            break;
        case VMAP::VMAP_LOAD_RESULT_ERROR:
            TC_LOG_INFO("maps", "Could not load VMAP name:%s, id:%d, x:%d, y:%d (vmap rep.: x:%d, y:%d)", GetMapName(), GetId(), gx, gy, gx, gy);
            break;
        case VMAP::VMAP_LOAD_RESULT_IGNORED:
            TC_LOG_DEBUG("maps", "Ignored VMAP name:%s, id:%d, x:%d, y:%d (vmap rep.: x:%d, y:%d)", GetMapName(), GetId(), gx, gy, gx, gy);
            break;
    }
}

void Map::LoadMap(int gx, int gy, bool reload)
{
    if (i_InstanceId != 0)
    {
        if (i_gridMaps[gx][gy])
            return;

        // load grid map for base map
        if (!m_parentMap->i_gridMaps[gx][gy])
            m_parentMap->EnsureGridCreated_i(GridCoord(63-gx, 63-gy));

        ((MapInstanced*)(m_parentMap))->AddGridMapReference(GridCoord(gx, gy));
        i_gridMaps[gx][gy] = m_parentMap->i_gridMaps[gx][gy];
        return;
    }

    if (i_gridMaps[gx][gy] && !reload)
        return;

    //map already load, delete it before reloading (Is it necessary? Do we really need the ability the reload maps during runtime?)
    if (i_gridMaps[gx][gy])
    {
        TC_LOG_DEBUG("maps", "Unloading previously loaded map %u before reloading.", GetId());

        delete i_gridMaps[gx][gy];
        i_gridMaps[gx][gy]=NULL;
    }

    // map file name
    char *tmp=NULL;
    int len = sWorld->GetDataPath().length()+strlen("maps/%03u%02u%02u.map")+1;
    tmp = new char[len];
    snprintf(tmp, len, (char *)(sWorld->GetDataPath()+"maps/%03u%02u%02u.map").c_str(), GetId(), gx, gy);
    TC_LOG_INFO("maps", "Loading map %s", tmp);
    // loading data
    i_gridMaps[gx][gy] = new GridMap();
    if (!i_gridMaps[gx][gy]->loadData(tmp))
    {
        TC_LOG_ERROR("maps", "Error loading map file: \n %s\n", tmp);
    }
    delete [] tmp;
}

void Map::LoadMapAndVMap(int gx, int gy)
{
    LoadMap(gx, gy);
    if (i_InstanceId == 0)
        LoadVMap(gx, gy);                                   // Only load the data for the base map
}

Map::Map(uint32 id, time_t expiry, uint32 InstanceId, uint8 SpawnMode,
         Map* _parent) :
    _creatureToMoveLock(false), i_mapEntry (sMapStore.LookupEntry(id)),
    i_spawnMode(SpawnMode), i_InstanceId(InstanceId), m_unloadTimer(0),
    m_VisibleDistance(DEFAULT_VISIBILITY_DISTANCE),
    m_VisibilityNotifyPeriod(DEFAULT_VISIBILITY_NOTIFY_PERIOD),
    i_gridExpiry(expiry), i_grids(), i_gridMaps(), i_scriptLock(false), _summonsInTimePeriod(0), _lastSummonTime(time(NULL))
{
    m_parentMap = (_parent ? _parent : this);
    Map::InitVisibilityDistance();
}

void Map::InitVisibilityDistance()
{
    //init visibility for continents
    m_VisibleDistance = sWorld->GetMaxVisibleDistanceOnContinents();
    m_VisibilityNotifyPeriod = sWorld->GetVisibilityNotifyPeriodOnContinents();
}

void Map::AddToGrid(Player *obj, Cell const &cell)
{
    auto const ngrid = getNGrid(cell.GridX(), cell.GridY());
    ngrid->GetGrid(cell.CellX(), cell.CellY()).AddWorldObject(obj);
}

void Map::AddToGrid(GameObject *obj, Cell const &cell)
{
    auto const ngrid = getNGrid(cell.GridX(), cell.GridY());
    ngrid->GetGrid(cell.CellX(), cell.CellY()).AddGridObject(obj);
}

void Map::AddToGrid(Creature *obj, Cell const &cell)
{
    auto const ngrid = getNGrid(cell.GridX(), cell.GridY());
    if (obj->IsWorldObject())
        ngrid->GetGrid(cell.CellX(), cell.CellY()).AddWorldObject(obj);
    else
        ngrid->GetGrid(cell.CellX(), cell.CellY()).AddGridObject(obj);

    obj->SetCurrentCell(cell);
}

void Map::SwitchGridContainers(Creature* obj, bool on)
{
    ASSERT(!obj->IsPermanentWorldObject());
    CellCoord p = Trinity::ComputeCellCoord(obj->GetPositionX(), obj->GetPositionY());
    if (!p.IsCoordValid())
    {
        TC_LOG_ERROR("maps", "Map::SwitchGridContainers: Object " UI64FMTD " has invalid coordinates X:%f Y:%f grid cell [%u:%u]", obj->GetGUID(), obj->GetPositionX(), obj->GetPositionY(), p.x_coord, p.y_coord);
        return;
    }

    Cell cell(p);
    if (!IsGridLoaded(GridCoord(cell.data.Part.grid_x, cell.data.Part.grid_y)))
        return;

    TC_LOG_DEBUG("maps", "Switch object " UI64FMTD " from grid[%u, %u] %u", obj->GetGUID(), cell.data.Part.grid_x, cell.data.Part.grid_y, on);

    auto const ngrid = getNGrid(cell.GridX(), cell.GridY());
    ASSERT(ngrid != nullptr);

    auto &grid = ngrid->GetGrid(cell.CellX(), cell.CellY());

    obj->RemoveFromGrid();

    if (on)
    {
        grid.AddWorldObject(obj);
        AddWorldObject(obj);
    }
    else
    {
        grid.AddGridObject(obj);
        RemoveWorldObject(obj);
    }

    obj->m_isTempWorldObject = on;
}

template<class T>
void Map::DeleteFromWorld(T* obj)
{
    // Note: In case resurrectable corpse and pet its removed from global lists in own destructor
    delete obj;
}

template<>
void Map::DeleteFromWorld(Player* player)
{
    sObjectAccessor->RemoveObject(player);
    sObjectAccessor->RemoveUpdateObject(player); //TODO: I do not know why we need this, it should be removed in ~Object anyway
    delete player;
}

//Create NGrid so the object can be added to it
//But object data is not loaded here
void Map::EnsureGridCreated(const GridCoord &p)
{
    if (!getNGrid(p.x_coord, p.y_coord))
    {
        GridGuardType guard(i_gridLock);
        EnsureGridCreated_i(p);
    }
}

//Create NGrid so the object can be added to it
//But object data is not loaded here
void Map::EnsureGridCreated_i(const GridCoord &p)
{
    if (getNGrid(p.x_coord, p.y_coord))
        return;

    // Such way allows to retrieve hint position for insert, so it
    // will have amortized constant complexity instead of logarithmic
    size_t const key = p.x_coord * MAX_NUMBER_OF_GRIDS + p.y_coord;
    GridContainerType::iterator lb = i_loadedGrids.lower_bound(key);

    if (lb != i_loadedGrids.end() && !i_loadedGrids.key_comp()(key, lb->first))
        return;

    TC_LOG_DEBUG("maps", "Creating grid[%u, %u] for map %u instance %u",
                 p.x_coord, p.y_coord, GetId(), i_InstanceId);

    auto const itr = i_loadedGrids.emplace_hint(lb,
        std::piecewise_construct,
        std::forward_as_tuple(key),
        std::forward_as_tuple(p.x_coord, p.y_coord, i_gridExpiry, sWorld->getBoolConfig(CONFIG_GRID_UNLOAD)));

    auto &ngrid = itr->second;

    ngrid.SetGridState(GRID_STATE_IDLE);
    setNGrid(&ngrid, p.x_coord, p.y_coord);

    //z coord
    int gx = (MAX_NUMBER_OF_GRIDS - 1) - p.x_coord;
    int gy = (MAX_NUMBER_OF_GRIDS - 1) - p.y_coord;

    if (!i_gridMaps[gx][gy])
        LoadMapAndVMap(gx, gy);
}

//Load NGrid and make it active
void Map::EnsureGridLoadedForActiveObject(const Cell &cell, WorldObject* object)
{
    EnsureGridLoaded(cell);
    auto const ngrid = getNGrid(cell.GridX(), cell.GridY());
    ASSERT(ngrid != nullptr);

    // refresh grid state & timer
    if (ngrid->GetGridState() != GRID_STATE_ACTIVE)
    {
        TC_LOG_DEBUG("maps", "Active object " UI64FMTD " triggers loading of grid [%u, %u] on map %u", object->GetGUID(), cell.GridX(), cell.GridY(), GetId());
        ResetGridExpiry(*ngrid, 0.1f);
        ngrid->SetGridState(GRID_STATE_ACTIVE);
    }
}

//Create NGrid and load the object data in it
bool Map::EnsureGridLoaded(const Cell &cell)
{
    EnsureGridCreated(GridCoord(cell.GridX(), cell.GridY()));

    auto const ngrid = getNGrid(cell.GridX(), cell.GridY());
    ASSERT(ngrid != nullptr);

    if (ngrid->isGridObjectDataLoaded())
        return false;

    TC_LOG_DEBUG("maps", "Loading grid[%u, %u] for map %u instance %u", cell.GridX(), cell.GridY(), GetId(), i_InstanceId);

    ngrid->setGridObjectDataLoaded(true);

    Trinity::ObjectGridLoader::LoadN(*ngrid, this, cell);

    // Add resurrectable corpses to world object list in grid
    sObjectAccessor->AddCorpsesToGrid(GridCoord(cell.GridX(), cell.GridY()), ngrid->GetGrid(cell.CellX(), cell.CellY()), this);

    Balance();
    return true;
}

void Map::LoadGrid(float x, float y)
{
    EnsureGridLoaded(Cell(x, y));
}

bool Map::AddPlayerToMap(Player* player)
{
    CellCoord cellCoord = Trinity::ComputeCellCoord(player->GetPositionX(), player->GetPositionY());
    if (!cellCoord.IsCoordValid())
    {
        TC_LOG_ERROR("maps", "Map::Add: Player (GUID: %u) has invalid coordinates X:%f Y:%f grid cell [%u:%u]", player->GetGUIDLow(), player->GetPositionX(), player->GetPositionY(), cellCoord.x_coord, cellCoord.y_coord);
        return false;
    }

    Cell cell(cellCoord);
    EnsureGridLoadedForActiveObject(cell, player);
    AddToGrid(player, cell);

    // Check if we are adding to correct map
    ASSERT (player->GetMap() == this);
    player->SetMap(this);
    player->AddToWorld();

    SendInitSelf(player);
    SendInitTransports(player);

    player->m_clientGUIDs.clear();
    player->OnRelocated();

    sScriptMgr->OnPlayerEnterMap(this, player);
    return true;
}

template<class T>
void Map::InitializeObject(T* /*obj*/)
{
}

template<>
void Map::InitializeObject(Creature* obj)
{
    obj->_moveState = CREATURE_CELL_MOVE_NONE;
}

template<class T>
bool Map::AddToMap(T *obj)
{
    //TODO: Needs clean up. An object should not be added to map twice.
    if (obj->IsInWorld())
    {
        ASSERT(obj->IsInGrid());
        obj->UpdateObjectVisibility(true);
        return true;
    }

    CellCoord cellCoord = Trinity::ComputeCellCoord(obj->GetPositionX(), obj->GetPositionY());
    //It will create many problems (including crashes) if an object is not added to grid after creation
    //The correct way to fix it is to make AddToMap return false and delete the object if it is not added to grid
    //But now AddToMap is used in too many places, I will just see how many ASSERT failures it will cause
    ASSERT(cellCoord.IsCoordValid());
    if (!cellCoord.IsCoordValid())
    {
        TC_LOG_ERROR("maps", "Map::Add: Object " UI64FMTD " has invalid coordinates X:%f Y:%f grid cell [%u:%u]", obj->GetGUID(), obj->GetPositionX(), obj->GetPositionY(), cellCoord.x_coord, cellCoord.y_coord);
        return false; //Should delete object
    }

    Cell cell(cellCoord);
    if (obj->isActiveObject())
        EnsureGridLoadedForActiveObject(cell, obj);
    else
        EnsureGridCreated(GridCoord(cell.GridX(), cell.GridY()));
    AddToGrid(obj, cell);
    TC_LOG_DEBUG("maps", "Object %u enters grid[%u, %u]", GUID_LOPART(obj->GetGUID()), cell.GridX(), cell.GridY());

    //Must already be set before AddToMap. Usually during obj->Create.
    //obj->SetMap(this);
    obj->AddToWorld();

    InitializeObject(obj);

    if (obj->isActiveObject())
        AddToActive(obj);

    //something, such as vehicle, needs to be update immediately
    //also, trigger needs to cast spell, if not update, cannot see visual
    obj->UpdateObjectVisibility(true);

    if (Creature* creature = obj->ToCreature())
        sBattlePetSpawnMgr->OnAddToOrRemoveFromMap(creature);

    return true;
}

bool Map::IsGridLoaded(const GridCoord &p) const
{
    auto const ngrid = getNGrid(p.x_coord, p.y_coord);
    return ngrid && ngrid->isGridObjectDataLoaded();
}

void Map::Update(const uint32 diff)
{
    // for creature
    auto gridObjectUpdate(Trinity::makeGridVisitor(i_objectUpdater));
    // for pets
    auto worldObjectUpdate(Trinity::makeWorldVisitor(i_objectUpdater));

    TC_PROBE1(worldserver, map_update.begin, this);

    _dynamicTree.update(diff);
    TC_PROBE1(worldserver, map_update.dynamic_tree.end, this);

    // update active cells around players and active objects
    resetMarkedCells();
    TC_PROBE1(worldserver, map_update.marked_cells.end, this);

    // update worldsessions for existing players
    for (m_mapRefIter = m_mapRefManager.begin(); m_mapRefIter != m_mapRefManager.end(); ++m_mapRefIter)
    {
        if (Player* player = m_mapRefIter->GetSource())
        {
            TC_PROBE2(worldserver, map_update.player.begin, this, player);

            player->GetSession()->Update(diff, MapSessionFilter(player->GetSession()));
            TC_PROBE2(worldserver, map_update.player_session.end, this, player);

            // Can be not in world after WorldSession::Update
            if (player->IsInWorld())
            {
                player->Update(diff);
                TC_PROBE2(worldserver, map_update.player_update.end, this, player);

                VisitNearbyCellsOf(this, player, gridObjectUpdate, worldObjectUpdate);
                TC_PROBE2(worldserver, map_update.player_cells.end, this, player);
            }
        }
    }
    TC_PROBE1(worldserver, map_update.players_loop.end, this);

    // non-player active objects, increasing iterator in the loop in case of object removal
    for (auto &obj : m_activeNonPlayers)
    {
        if (obj && obj->IsInWorld())
        {
            TC_PROBE2(worldserver, map_update.object_cells.begin, this, obj);
            VisitNearbyCellsOf(this, obj, gridObjectUpdate, worldObjectUpdate);
            TC_PROBE2(worldserver, map_update.object_cells.end, this, obj);
        }
    }
    TC_PROBE1(worldserver, map_update.objects_loop.end, this);

    i_objectUpdater.updateCollected(diff);
    TC_PROBE1(worldserver, map_update.collected.end, this);

    ///- Process necessary scripts
    if (!m_scriptSchedule.empty())
    {
        i_scriptLock = true;
        ScriptsProcess();
        i_scriptLock = false;
    }
    TC_PROBE1(worldserver, map_update.scripts.end, this);

    MoveAllCreaturesInMoveList();
    TC_PROBE1(worldserver, map_update.move_list.end, this);
}

void Map::RemovePlayerFromMap(Player* player, bool remove)
{
    sScriptMgr->OnPlayerLeaveMap(this, player);

    player->RemoveFromWorld();
    SendRemoveTransports(player);

    player->UpdateObjectVisibility(true);
    if (player->IsInGrid())
        player->RemoveFromGrid();
    else
        ASSERT(remove); //maybe deleted in logoutplayer when player is not in a map

    if (remove)
        DeleteFromWorld(player);
}

template<class T>
void Map::RemoveFromMap(T *obj, bool remove)
{
    if (Creature* creature = obj->ToCreature())
        sBattlePetSpawnMgr->OnAddToOrRemoveFromMap(creature, true);

    obj->RemoveFromWorld();
    if (obj->isActiveObject())
        RemoveFromActive(obj);

    obj->UpdateObjectVisibility(true);
    obj->RemoveFromGrid();
    obj->ResetMap();

    if (remove)
    {
        // if option set then object already saved at this moment
        if (!sWorld->getBoolConfig(CONFIG_SAVE_RESPAWN_TIME_IMMEDIATELY))
            obj->SaveRespawnTime();
        DeleteFromWorld(obj);
    }
}

void Map::PlayerRelocation(Player* player, float x, float y, float z, float orientation)
{
    ASSERT(player);

    Cell old_cell(player->GetPositionX(), player->GetPositionY());
    Cell new_cell(x, y);

    //! If hovering, always increase our server-side Z position
    //! Client automatically projects correct position based on Z coord sent in monster move
    //! and UNIT_FIELD_HOVERHEIGHT sent in object updates
    if (player->HasUnitMovementFlag(MOVEMENTFLAG_HOVER))
        z += player->GetFloatValue(UNIT_FIELD_HOVERHEIGHT);

    player->Relocate(x, y, z, orientation);
    if (player->IsVehicle())
        player->GetVehicleKit()->RelocatePassengers();

    if (old_cell.DiffGrid(new_cell) || old_cell.DiffCell(new_cell))
    {
        TC_LOG_DEBUG("maps", "Player %s relocation grid[%u, %u]cell[%u, %u]->grid[%u, %u]cell[%u, %u]",
                     player->GetName().c_str(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(),
                     new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());

        player->RemoveFromGrid();

        if (old_cell.DiffGrid(new_cell))
            EnsureGridLoadedForActiveObject(new_cell, player);

        AddToGrid(player, new_cell);
    }

    player->OnRelocated();
}

void Map::CreatureRelocation(Creature* creature, float x, float y, float z, float ang, bool respawnRelocationOnFail)
{
    Cell const old_cell(creature->GetCurrentCell());
    Cell new_cell(x, y);

    if (!respawnRelocationOnFail && !getNGrid(new_cell.GridX(), new_cell.GridY()))
        return;

    //! If hovering, always increase our server-side Z position
    //! Client automatically projects correct position based on Z coord sent in monster move
    //! and UNIT_FIELD_HOVERHEIGHT sent in object updates
    if (creature->HasUnitMovementFlag(MOVEMENTFLAG_HOVER))
        z += creature->GetFloatValue(UNIT_FIELD_HOVERHEIGHT);

    // delay creature move for grid/cell to grid/cell moves
    if (old_cell.DiffCell(new_cell) || old_cell.DiffGrid(new_cell))
    {
        #ifdef TRINITY_DEBUG
            TC_LOG_DEBUG("maps", "Creature (GUID: %u Entry: %u) added to moving list from grid[%u, %u]cell[%u, %u] to grid[%u, %u]cell[%u, %u].", creature->GetGUIDLow(), creature->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());
        #endif
        AddCreatureToMoveList(creature, x, y, z, ang);
        // in diffcell/diffgrid case notifiers called at finishing move creature in Map::MoveAllCreaturesInMoveList
    }
    else
    {
        creature->Relocate(x, y, z, ang);
        if (creature->IsVehicle())
            creature->GetVehicleKit()->RelocatePassengers();
        RemoveCreatureFromMoveList(creature);
        creature->OnRelocated();
    }
}

void Map::AddCreatureToMoveList(Creature* c, float x, float y, float z, float ang)
{
    if (_creatureToMoveLock) //can this happen?
        return;

    if (c->_moveState == CREATURE_CELL_MOVE_NONE)
        _creaturesToMove.push_back(c);
    c->SetNewCellPosition(x, y, z, ang);
}

void Map::RemoveCreatureFromMoveList(Creature* c)
{
    if (_creatureToMoveLock) //can this happen?
        return;

    if (c->_moveState == CREATURE_CELL_MOVE_ACTIVE)
        c->_moveState = CREATURE_CELL_MOVE_INACTIVE;
}

void Map::MoveAllCreaturesInMoveList()
{
    _creatureToMoveLock = true;
    for (std::vector<Creature*>::iterator itr = _creaturesToMove.begin(); itr != _creaturesToMove.end(); ++itr)
    {
        Creature* c = *itr;
        if (c->FindMap() != this) //pet is teleported to another map
            continue;

        if (c->_moveState != CREATURE_CELL_MOVE_ACTIVE)
        {
            c->_moveState = CREATURE_CELL_MOVE_NONE;
            continue;
        }

        c->_moveState = CREATURE_CELL_MOVE_NONE;
        if (!c->IsInWorld())
            continue;

        // do move or do move to respawn or remove creature if previous all fail
        if (CreatureCellRelocation(c, Cell(c->_newPosition.m_positionX, c->_newPosition.m_positionY)))
        {
            // update pos
            c->Relocate(c->_newPosition);
            c->UpdateObjectVisibility(false);
        }
        else
        {
            // if creature can't be move in new cell/grid (not loaded) move it to repawn cell/grid
            // creature coordinates will be updated and notifiers send
            if (!CreatureRespawnRelocation(c, false))
            {
                // ... or unload (if respawn grid also not loaded)
                #ifdef TRINITY_DEBUG
                    TC_LOG_DEBUG("maps", "Creature (GUID: %u Entry: %u) cannot be move to unloaded respawn grid.", c->GetGUIDLow(), c->GetEntry());
                #endif
                //AddObjectToRemoveList(Pet*) should only be called in Pet::Remove
                //This may happen when a player just logs in and a pet moves to a nearby unloaded cell
                //To avoid this, we can load nearby cells when player log in
                //But this check is always needed to ensure safety
                //TODO: pets will disappear if this is outside CreatureRespawnRelocation
                //need to check why pet is frequently relocated to an unloaded cell
                if (c->isPet())
                    ((Pet*)c)->Remove(PET_REMOVE_DISMISS, PET_REMOVE_FLAG_RETURN_REAGENT);
                else
                    AddObjectToRemoveList(c);
            }
        }
    }
    _creaturesToMove.clear();
    _creatureToMoveLock = false;
}

bool Map::CreatureCellRelocation(Creature* c, Cell new_cell)
{
    Cell const old_cell(c->GetCurrentCell());
    if (!old_cell.DiffGrid(new_cell))                       // in same grid
    {
        // if in same cell then none do
        if (old_cell.DiffCell(new_cell))
        {
            #ifdef TRINITY_DEBUG
                TC_LOG_DEBUG("maps", "Creature (GUID: %u Entry: %u) moved in grid[%u, %u] from cell[%u, %u] to cell[%u, %u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.CellX(), new_cell.CellY());
            #endif

            c->RemoveFromGrid();
            AddToGrid(c, new_cell);
        }
        else
        {
            #ifdef TRINITY_DEBUG
                TC_LOG_DEBUG("maps", "Creature (GUID: %u Entry: %u) moved in same grid[%u, %u]cell[%u, %u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY());
            #endif
        }

        return true;
    }

    // in diff. grids but active creature
    if (c->isActiveObject())
    {
        EnsureGridLoadedForActiveObject(new_cell, c);

        #ifdef TRINITY_DEBUG
            TC_LOG_DEBUG("maps", "Active creature (GUID: %u Entry: %u) moved from grid[%u, %u]cell[%u, %u] to grid[%u, %u]cell[%u, %u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());
        #endif

        c->RemoveFromGrid();
        AddToGrid(c, new_cell);

        return true;
    }

    // in diff. loaded grid normal creature
    if (IsGridLoaded(GridCoord(new_cell.GridX(), new_cell.GridY())))
    {
        #ifdef TRINITY_DEBUG
            TC_LOG_DEBUG("maps", "Creature (GUID: %u Entry: %u) moved from grid[%u, %u]cell[%u, %u] to grid[%u, %u]cell[%u, %u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());
        #endif

        c->RemoveFromGrid();
        EnsureGridCreated(GridCoord(new_cell.GridX(), new_cell.GridY()));
        AddToGrid(c, new_cell);

        return true;
    }

    // fail to move: normal creature attempt move to unloaded grid
    #ifdef TRINITY_DEBUG
        TC_LOG_DEBUG("maps", "Creature (GUID: %u Entry: %u) attempted to move from grid[%u, %u]cell[%u, %u] to unloaded grid[%u, %u]cell[%u, %u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());
    #endif
    return false;
}

bool Map::CreatureRespawnRelocation(Creature* c, bool diffGridOnly)
{
    float resp_x, resp_y, resp_z, resp_o;
    c->GetRespawnPosition(resp_x, resp_y, resp_z, &resp_o);

    Cell const old_cell(c->GetCurrentCell());
    Cell resp_cell(resp_x, resp_y);

    //creature will be unloaded with grid
    if (diffGridOnly && !old_cell.DiffGrid(resp_cell))
        return true;

    c->CombatStop();
    c->GetMotionMaster()->Clear();

    #ifdef TRINITY_DEBUG
        TC_LOG_DEBUG("maps", "Creature (GUID: %u Entry: %u) moved from grid[%u, %u]cell[%u, %u] to respawn grid[%u, %u]cell[%u, %u].",
                     c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(),
                     resp_cell.GridX(), resp_cell.GridY(), resp_cell.CellX(), resp_cell.CellY());
    #endif

    // teleport it to respawn point (like normal respawn if player see)
    if (CreatureCellRelocation(c, resp_cell))
    {
        c->Relocate(resp_x, resp_y, resp_z, resp_o);
        c->GetMotionMaster()->Initialize();                 // prevent possible problems with default move generators
        //CreatureRelocationNotify(c, resp_cell, resp_cell.GetCellCoord());
        c->UpdateObjectVisibility(false);
        return true;
    }

    return false;
}

bool Map::UnloadGrid(GridContainerType::iterator itr, bool unloadAll)
{
    auto &ngrid = itr->second;

    auto const x = ngrid.getX();
    auto const y = ngrid.getY();

    {
        if (!unloadAll)
        {
            // pets, possessed creatures (must be active), transport passengers
            if (ngrid.GetWorldObjectCountInNGrid<Creature>())
                return false;

            if (ActiveObjectsNearGrid(ngrid))
                return false;
        }

        TC_LOG_DEBUG("maps", "Unloading grid[%u, %u] for map %u", x, y, GetId());

        if (!unloadAll)
        {
            // Finish creature moves, remove and delete all creatures with delayed remove before moving to respawn grids
            // Must know real mob position before move
            MoveAllCreaturesInMoveList();

            // move creatures to respawn grids if this is diff.grid or to remove list
            Trinity::ObjectGridEvacuator worker;
            ngrid.VisitAllGrids(Trinity::makeGridVisitor(worker));

            // Finish creature moves, remove and delete all creatures with delayed remove before unload
            MoveAllCreaturesInMoveList();
        }

        {
            Trinity::ObjectGridCleaner worker;
            ngrid.VisitAllGrids(Trinity::makeGridVisitor(worker));
        }

        RemoveAllObjectsInRemoveList();

        {
            Trinity::ObjectGridUnloader worker;
            ngrid.VisitAllGrids(Trinity::makeGridVisitor(worker));
        }

        ASSERT(i_objectsToRemove.empty());

        {
            // Resurrectable corpses are cached in ObjectAccessor and must be
            // removed from grid whenever grid is unloaded to avoid dangling
            // pointers
            CorpseGridReset worker;
            ngrid.VisitAllGrids(Trinity::makeWorldVisitor(worker));
        }

        i_loadedGrids.erase(itr);
        setNGrid(NULL, x, y);
    }

    int gx = (MAX_NUMBER_OF_GRIDS - 1) - x;
    int gy = (MAX_NUMBER_OF_GRIDS - 1) - y;

    // delete grid map, but don't delete if it is from parent map (and thus only reference)
    //+++if (GridMaps[gx][gy]) don't check for GridMaps[gx][gy], we might have to unload vmaps
    {
        if (i_InstanceId == 0)
        {
            if (i_gridMaps[gx][gy])
            {
                i_gridMaps[gx][gy]->unloadData();
                delete i_gridMaps[gx][gy];
            }
            // x and y are swapped
            VMAP::VMapFactory::createOrGetVMapManager()->unloadMap(GetId(), gx, gy);
        }
        else
            ((MapInstanced*)m_parentMap)->RemoveGridMapReference(GridCoord(gx, gy));

        i_gridMaps[gx][gy] = NULL;
    }
    TC_LOG_DEBUG("maps", "Unloading grid[%u, %u] for map %u finished", x, y, GetId());
    return true;
}

void Map::RemoveAllPlayers()
{
    if (HavePlayers())
    {
        for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        {
            Player* player = itr->GetSource();
            if (!player->IsBeingTeleportedFar())
            {
                // this is happening for bg
                TC_LOG_ERROR("maps", "Map::UnloadAll: player %s is still in map %u during unload, this should not happen!",
                             player->GetName().c_str(), GetId());
                player->TeleportTo(player->m_homebindMapId, player->m_homebindX, player->m_homebindY, player->m_homebindZ, player->GetOrientation());
            }
        }
    }
}

void Map::UnloadAll()
{
    // clear all delayed moves, useless anyway do these moves before map unload.
    _creaturesToMove.clear();

    for (auto i = i_loadedGrids.begin(); i != i_loadedGrids.end();)
        UnloadGrid(i++, true);
}

// *****************************
// Grid function
// *****************************
GridMap::GridMap()
{
    _flags = 0;
    // Area data
    _gridArea = 0;
    _areaMap = NULL;
    // Height level data
    _gridHeight = INVALID_HEIGHT;
    _gridGetHeight = &GridMap::getHeightFromFlat;
    m_V9 = NULL;
    m_V8 = NULL;
    // Liquid data
    _liquidType    = 0;
    _liquidOffX   = 0;
    _liquidOffY   = 0;
    _liquidWidth  = 0;
    _liquidHeight = 0;
    _liquidLevel = INVALID_HEIGHT;
    _liquidEntry = NULL;
    _liquidFlags = NULL;
    _liquidMap  = NULL;
}

GridMap::~GridMap()
{
    unloadData();
}

bool GridMap::loadData(char *filename)
{
    // Unload old data if exist
    unloadData();

    map_fileheader header;
    // Not return error if file not found
    FILE* in = fopen(filename, "rb");
    if (!in)
        return true;

    if (fread(&header, sizeof(header), 1, in) != 1)
    {
        fclose(in);
        return false;
    }

    if (header.mapMagic == MapMagic.asUInt && header.versionMagic == MapVersionMagic.asUInt)
    {
        // loadup area data
        if (header.areaMapOffset && !loadAreaData(in, header.areaMapOffset, header.areaMapSize))
        {
            TC_LOG_ERROR("maps", "Error loading map area data\n");
            fclose(in);
            return false;
        }
        // loadup height data
        if (header.heightMapOffset && !loadHeihgtData(in, header.heightMapOffset, header.heightMapSize))
        {
            TC_LOG_ERROR("maps", "Error loading map height data\n");
            fclose(in);
            return false;
        }
        // loadup liquid data
        if (header.liquidMapOffset && !loadLiquidData(in, header.liquidMapOffset, header.liquidMapSize))
        {
            TC_LOG_ERROR("maps", "Error loading map liquids data\n");
            fclose(in);
            return false;
        }
        fclose(in);
        return true;
    }
    TC_LOG_ERROR("maps", "Map file '%s' is from an incompatible clientversion. Please recreate using the mapextractor.", filename);
    fclose(in);
    return false;
}

void GridMap::unloadData()
{
    delete[] _areaMap;
    delete[] m_V9;
    delete[] m_V8;
    delete[] _liquidEntry;
    delete[] _liquidFlags;
    delete[] _liquidMap;
    _areaMap = NULL;
    m_V9 = NULL;
    m_V8 = NULL;
    _liquidEntry = NULL;
    _liquidFlags = NULL;
    _liquidMap  = NULL;
    _gridGetHeight = &GridMap::getHeightFromFlat;
}

bool GridMap::loadAreaData(FILE* in, uint32 offset, uint32 /*size*/)
{
    map_areaHeader header;
    fseek(in, offset, SEEK_SET);

    if (fread(&header, sizeof(header), 1, in) != 1 || header.fourcc != MapAreaMagic.asUInt)
        return false;

    _gridArea = header.gridArea;
    if (!(header.flags & MAP_AREA_NO_AREA))
    {
        _areaMap = new uint16 [16*16];
        if (fread(_areaMap, sizeof(uint16), 16*16, in) != 16*16)
            return false;
    }
    return true;
}

bool GridMap::loadHeihgtData(FILE* in, uint32 offset, uint32 /*size*/)
{
    map_heightHeader header;
    fseek(in, offset, SEEK_SET);

    if (fread(&header, sizeof(header), 1, in) != 1 || header.fourcc != MapHeightMagic.asUInt)
        return false;

    _gridHeight = header.gridHeight;
    if (!(header.flags & MAP_HEIGHT_NO_HEIGHT))
    {
        if ((header.flags & MAP_HEIGHT_AS_INT16))
        {
            m_uint16_V9 = new uint16 [129*129];
            m_uint16_V8 = new uint16 [128*128];
            if (fread(m_uint16_V9, sizeof(uint16), 129*129, in) != 129*129 ||
                fread(m_uint16_V8, sizeof(uint16), 128*128, in) != 128*128)
                return false;
            _gridIntHeightMultiplier = (header.gridMaxHeight - header.gridHeight) / 65535;
            _gridGetHeight = &GridMap::getHeightFromUint16;
        }
        else if ((header.flags & MAP_HEIGHT_AS_INT8))
        {
            m_uint8_V9 = new uint8 [129*129];
            m_uint8_V8 = new uint8 [128*128];
            if (fread(m_uint8_V9, sizeof(uint8), 129*129, in) != 129*129 ||
                fread(m_uint8_V8, sizeof(uint8), 128*128, in) != 128*128)
                return false;
            _gridIntHeightMultiplier = (header.gridMaxHeight - header.gridHeight) / 255;
            _gridGetHeight = &GridMap::getHeightFromUint8;
        }
        else
        {
            m_V9 = new float [129*129];
            m_V8 = new float [128*128];
            if (fread(m_V9, sizeof(float), 129*129, in) != 129*129 ||
                fread(m_V8, sizeof(float), 128*128, in) != 128*128)
                return false;
            _gridGetHeight = &GridMap::getHeightFromFloat;
        }
    }
    else
        _gridGetHeight = &GridMap::getHeightFromFlat;
    return true;
}

bool GridMap::loadLiquidData(FILE* in, uint32 offset, uint32 /*size*/)
{
    map_liquidHeader header;
    fseek(in, offset, SEEK_SET);

    if (fread(&header, sizeof(header), 1, in) != 1 || header.fourcc != MapLiquidMagic.asUInt)
        return false;

    _liquidType   = header.liquidType;
    _liquidOffX  = header.offsetX;
    _liquidOffY  = header.offsetY;
    _liquidWidth = header.width;
    _liquidHeight = header.height;
    _liquidLevel  = header.liquidLevel;

    if (!(header.flags & MAP_LIQUID_NO_TYPE))
    {
        _liquidEntry = new uint16[16*16];
        if (fread(_liquidEntry, sizeof(uint16), 16*16, in) != 16*16)
            return false;

        _liquidFlags = new uint8[16*16];
        if (fread(_liquidFlags, sizeof(uint8), 16*16, in) != 16*16)
            return false;
    }
    if (!(header.flags & MAP_LIQUID_NO_HEIGHT))
    {
        _liquidMap = new float[uint32(_liquidWidth) * uint32(_liquidHeight)];
          if (fread(_liquidMap, sizeof(float), _liquidWidth*_liquidHeight, in) != (uint32(_liquidWidth) * uint32(_liquidHeight)))
            return false;
    }
    return true;
}

uint16 GridMap::getArea(float x, float y) const
{
    if (!_areaMap)
        return _gridArea;

    x = 16 * (32 - x/SIZE_OF_GRIDS);
    y = 16 * (32 - y/SIZE_OF_GRIDS);
    int lx = (int)x & 15;
    int ly = (int)y & 15;
    return _areaMap[lx*16 + ly];
}

float GridMap::getHeightFromFlat(float /*x*/, float /*y*/) const
{
    return _gridHeight;
}

float GridMap::getHeightFromFloat(float x, float y) const
{
    if (!m_V8 || !m_V9)
        return _gridHeight;

    x = MAP_RESOLUTION * (32 - x/SIZE_OF_GRIDS);
    y = MAP_RESOLUTION * (32 - y/SIZE_OF_GRIDS);

    int x_int = (int)x;
    int y_int = (int)y;
    x -= x_int;
    y -= y_int;
    x_int&=(MAP_RESOLUTION - 1);
    y_int&=(MAP_RESOLUTION - 1);

    // Height stored as: h5 - its v8 grid, h1-h4 - its v9 grid
    // +--------------> X
    // | h1-------h2     Coordinates is:
    // | | \  1  / |     h1 0, 0
    // | |  \   /  |     h2 0, 1
    // | | 2  h5 3 |     h3 1, 0
    // | |  /   \  |     h4 1, 1
    // | | /  4  \ |     h5 1/2, 1/2
    // | h3-------h4
    // V Y
    // For find height need
    // 1 - detect triangle
    // 2 - solve linear equation from triangle points
    // Calculate coefficients for solve h = a*x + b*y + c

    float a, b, c;
    // Select triangle:
    if (x+y < 1)
    {
        if (x > y)
        {
            // 1 triangle (h1, h2, h5 points)
            float h1 = m_V9[(x_int)*129 + y_int];
            float h2 = m_V9[(x_int+1)*129 + y_int];
            float h5 = 2 * m_V8[x_int*128 + y_int];
            a = h2-h1;
            b = h5-h1-h2;
            c = h1;
        }
        else
        {
            // 2 triangle (h1, h3, h5 points)
            float h1 = m_V9[x_int*129 + y_int  ];
            float h3 = m_V9[x_int*129 + y_int+1];
            float h5 = 2 * m_V8[x_int*128 + y_int];
            a = h5 - h1 - h3;
            b = h3 - h1;
            c = h1;
        }
    }
    else
    {
        if (x > y)
        {
            // 3 triangle (h2, h4, h5 points)
            float h2 = m_V9[(x_int+1)*129 + y_int  ];
            float h4 = m_V9[(x_int+1)*129 + y_int+1];
            float h5 = 2 * m_V8[x_int*128 + y_int];
            a = h2 + h4 - h5;
            b = h4 - h2;
            c = h5 - h4;
        }
        else
        {
            // 4 triangle (h3, h4, h5 points)
            float h3 = m_V9[(x_int)*129 + y_int+1];
            float h4 = m_V9[(x_int+1)*129 + y_int+1];
            float h5 = 2 * m_V8[x_int*128 + y_int];
            a = h4 - h3;
            b = h3 + h4 - h5;
            c = h5 - h4;
        }
    }
    // Calculate height
    return a * x + b * y + c;
}

float GridMap::getHeightFromUint8(float x, float y) const
{
    if (!m_uint8_V8 || !m_uint8_V9)
        return _gridHeight;

    x = MAP_RESOLUTION * (32 - x/SIZE_OF_GRIDS);
    y = MAP_RESOLUTION * (32 - y/SIZE_OF_GRIDS);

    int x_int = (int)x;
    int y_int = (int)y;
    x -= x_int;
    y -= y_int;
    x_int&=(MAP_RESOLUTION - 1);
    y_int&=(MAP_RESOLUTION - 1);

    int32 a, b, c;
    uint8 *V9_h1_ptr = &m_uint8_V9[x_int*128 + x_int + y_int];
    if (x+y < 1)
    {
        if (x > y)
        {
            // 1 triangle (h1, h2, h5 points)
            int32 h1 = V9_h1_ptr[  0];
            int32 h2 = V9_h1_ptr[129];
            int32 h5 = 2 * m_uint8_V8[x_int*128 + y_int];
            a = h2-h1;
            b = h5-h1-h2;
            c = h1;
        }
        else
        {
            // 2 triangle (h1, h3, h5 points)
            int32 h1 = V9_h1_ptr[0];
            int32 h3 = V9_h1_ptr[1];
            int32 h5 = 2 * m_uint8_V8[x_int*128 + y_int];
            a = h5 - h1 - h3;
            b = h3 - h1;
            c = h1;
        }
    }
    else
    {
        if (x > y)
        {
            // 3 triangle (h2, h4, h5 points)
            int32 h2 = V9_h1_ptr[129];
            int32 h4 = V9_h1_ptr[130];
            int32 h5 = 2 * m_uint8_V8[x_int*128 + y_int];
            a = h2 + h4 - h5;
            b = h4 - h2;
            c = h5 - h4;
        }
        else
        {
            // 4 triangle (h3, h4, h5 points)
            int32 h3 = V9_h1_ptr[  1];
            int32 h4 = V9_h1_ptr[130];
            int32 h5 = 2 * m_uint8_V8[x_int*128 + y_int];
            a = h4 - h3;
            b = h3 + h4 - h5;
            c = h5 - h4;
        }
    }
    // Calculate height
    return (float)((a * x) + (b * y) + c)*_gridIntHeightMultiplier + _gridHeight;
}

float GridMap::getHeightFromUint16(float x, float y) const
{
    if (!m_uint16_V8 || !m_uint16_V9)
        return _gridHeight;

    x = MAP_RESOLUTION * (32 - x/SIZE_OF_GRIDS);
    y = MAP_RESOLUTION * (32 - y/SIZE_OF_GRIDS);

    int x_int = (int)x;
    int y_int = (int)y;
    x -= x_int;
    y -= y_int;
    x_int&=(MAP_RESOLUTION - 1);
    y_int&=(MAP_RESOLUTION - 1);

    int32 a, b, c;
    uint16 *V9_h1_ptr = &m_uint16_V9[x_int*128 + x_int + y_int];
    if (x+y < 1)
    {
        if (x > y)
        {
            // 1 triangle (h1, h2, h5 points)
            int32 h1 = V9_h1_ptr[  0];
            int32 h2 = V9_h1_ptr[129];
            int32 h5 = 2 * m_uint16_V8[x_int*128 + y_int];
            a = h2-h1;
            b = h5-h1-h2;
            c = h1;
        }
        else
        {
            // 2 triangle (h1, h3, h5 points)
            int32 h1 = V9_h1_ptr[0];
            int32 h3 = V9_h1_ptr[1];
            int32 h5 = 2 * m_uint16_V8[x_int*128 + y_int];
            a = h5 - h1 - h3;
            b = h3 - h1;
            c = h1;
        }
    }
    else
    {
        if (x > y)
        {
            // 3 triangle (h2, h4, h5 points)
            int32 h2 = V9_h1_ptr[129];
            int32 h4 = V9_h1_ptr[130];
            int32 h5 = 2 * m_uint16_V8[x_int*128 + y_int];
            a = h2 + h4 - h5;
            b = h4 - h2;
            c = h5 - h4;
        }
        else
        {
            // 4 triangle (h3, h4, h5 points)
            int32 h3 = V9_h1_ptr[  1];
            int32 h4 = V9_h1_ptr[130];
            int32 h5 = 2 * m_uint16_V8[x_int*128 + y_int];
            a = h4 - h3;
            b = h3 + h4 - h5;
            c = h5 - h4;
        }
    }
    // Calculate height
    return (float)((a * x) + (b * y) + c)*_gridIntHeightMultiplier + _gridHeight;
}

float GridMap::getLiquidLevel(float x, float y) const
{
    if (!_liquidMap)
        return _liquidLevel;

    x = MAP_RESOLUTION * (32 - x/SIZE_OF_GRIDS);
    y = MAP_RESOLUTION * (32 - y/SIZE_OF_GRIDS);

    int cx_int = ((int)x & (MAP_RESOLUTION-1)) - _liquidOffY;
    int cy_int = ((int)y & (MAP_RESOLUTION-1)) - _liquidOffX;

    if (cx_int < 0 || cx_int >=_liquidHeight)
        return INVALID_HEIGHT;
    if (cy_int < 0 || cy_int >=_liquidWidth)
        return INVALID_HEIGHT;

    return _liquidMap[cx_int*_liquidWidth + cy_int];
}

// Why does this return LIQUID data?
uint8 GridMap::getTerrainType(float x, float y) const
{
    if (!_liquidFlags)
        return 0;

    x = 16 * (32 - x/SIZE_OF_GRIDS);
    y = 16 * (32 - y/SIZE_OF_GRIDS);
    int lx = (int)x & 15;
    int ly = (int)y & 15;
    return _liquidFlags[lx*16 + ly];
}

// Get water state on map
inline ZLiquidStatus GridMap::getLiquidStatus(float x, float y, float z, uint8 ReqLiquidType, LiquidData* data)
{
    // Check water type (if no water return)
    if (!_liquidType && !_liquidFlags)
        return LIQUID_MAP_NO_WATER;

    // Get cell
    float cx = MAP_RESOLUTION * (32 - x/SIZE_OF_GRIDS);
    float cy = MAP_RESOLUTION * (32 - y/SIZE_OF_GRIDS);

    int x_int = (int)cx & (MAP_RESOLUTION-1);
    int y_int = (int)cy & (MAP_RESOLUTION-1);

    // Check water type in cell
    int idx=(x_int>>3)*16 + (y_int>>3);
    uint8 type = _liquidFlags ? _liquidFlags[idx] : _liquidType;
    uint32 entry = 0;
    if (_liquidEntry)
    {
        if (LiquidTypeEntry const* liquidEntry = sLiquidTypeStore.LookupEntry(_liquidEntry[idx]))
        {
            entry = liquidEntry->Id;
            type &= MAP_LIQUID_TYPE_DARK_WATER;
            uint32 liqTypeIdx = liquidEntry->Type;
            if (entry < 21)
            {
                if (AreaTableEntry const* area = GetAreaEntryByAreaFlagAndMap(getArea(x, y), MAPID_INVALID))
                {
                    uint32 overrideLiquid = area->LiquidTypeOverride[liquidEntry->Type];
                    if (!overrideLiquid && area->zone)
                    {
                        area = GetAreaEntryByAreaID(area->zone);
                        if (area)
                            overrideLiquid = area->LiquidTypeOverride[liquidEntry->Type];
                    }

                    if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(overrideLiquid))
                    {
                        entry = overrideLiquid;
                        liqTypeIdx = liq->Type;
                    }
                }
            }

            type |= 1 << liqTypeIdx;
        }
    }

    if (type == 0)
        return LIQUID_MAP_NO_WATER;

    // Check req liquid type mask
    if (ReqLiquidType && !(ReqLiquidType&type))
        return LIQUID_MAP_NO_WATER;

    // Check water level:
    // Check water height map
    int lx_int = x_int - _liquidOffY;
    int ly_int = y_int - _liquidOffX;
    if (lx_int < 0 || lx_int >=_liquidHeight)
        return LIQUID_MAP_NO_WATER;
    if (ly_int < 0 || ly_int >=_liquidWidth)
        return LIQUID_MAP_NO_WATER;

    // Get water level
    float liquid_level = _liquidMap ? _liquidMap[lx_int*_liquidWidth + ly_int] : _liquidLevel;
    // Get ground level (sub 0.2 for fix some errors)
    float ground_level = getHeight(x, y);

    // Check water level and ground level
    if (liquid_level < ground_level || z < ground_level - 2)
        return LIQUID_MAP_NO_WATER;

    // All ok in water -> store data
    if (data)
    {
        data->entry = entry;
        data->type_flags  = type;
        data->level = liquid_level;
        data->depth_level = ground_level;
    }

    // For speed check as int values
    float delta = liquid_level - z;

    if (delta > 2.0f)                   // Under water
        return LIQUID_MAP_UNDER_WATER;
    if (delta > 0.0f)                   // In water
        return LIQUID_MAP_IN_WATER;
    if (delta > -0.1f)                   // Walk on water
        return LIQUID_MAP_WATER_WALK;
                                      // Above water
    return LIQUID_MAP_ABOVE_WATER;
}

GridMap* Map::GetGrid(float x, float y)
{
    // half opt method
    int gx=(int)(32-x/SIZE_OF_GRIDS);                       //grid x
    int gy=(int)(32-y/SIZE_OF_GRIDS);                       //grid y

    if (gx >= MAX_NUMBER_OF_GRIDS || gy >= MAX_NUMBER_OF_GRIDS ||
        gx < 0 || gy < 0)
        return NULL;

    // ensure GridMap is loaded
    EnsureGridCreated(GridCoord(63-gx, 63-gy));

    return i_gridMaps[gx][gy];
}

float Map::GetWaterOrGroundLevel(float x, float y, float z, float* ground /*= NULL*/, bool /*swim = false*/) const
{
    if (const_cast<Map*>(this)->GetGrid(x, y))
    {
        // we need ground level (including grid height version) for proper return water level in point
        float ground_z = GetHeight(PHASEMASK_NORMAL, x, y, z, true, 50.0f);
        if (ground)
            *ground = ground_z;

        LiquidData liquid_status;

        ZLiquidStatus res = getLiquidStatus(x, y, ground_z, MAP_ALL_LIQUIDS, &liquid_status);
        return res ? liquid_status.level : ground_z;
    }

    return VMAP_INVALID_HEIGHT_VALUE;
}

float Map::GetHeight(float x, float y, float z, bool checkVMap /*= true*/, float maxSearchDist /*= DEFAULT_HEIGHT_SEARCH*/) const
{
    // find raw .map surface under Z coordinates
    float mapHeight = VMAP_INVALID_HEIGHT_VALUE;
    if (GridMap* gmap = const_cast<Map*>(this)->GetGrid(x, y))
    {
        float gridHeight = gmap->getHeight(x, y);
        // look from a bit higher pos to find the floor, ignore under surface case
        if (z + 2.0f > gridHeight)
            mapHeight = gridHeight;
    }

    float vmapHeight = VMAP_INVALID_HEIGHT_VALUE;
    if (checkVMap)
    {
        VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
        if (vmgr->isHeightCalcEnabled())
            vmapHeight = vmgr->getHeight(GetId(), x, y, z + 2.0f, maxSearchDist);   // look from a bit higher pos to find the floor
    }

    // mapHeight set for any above raw ground Z or <= INVALID_HEIGHT
    // vmapheight set for any under Z value or <= INVALID_HEIGHT
    if (vmapHeight > INVALID_HEIGHT)
    {
        if (mapHeight > INVALID_HEIGHT)
        {
            // we have mapheight and vmapheight and must select more appropriate

            // we are already under the surface or vmap height above map heigt
            // or if the distance of the vmap height is less the land height distance
            if (z < mapHeight || vmapHeight > mapHeight || fabs(mapHeight-z) > fabs(vmapHeight-z))
                return vmapHeight;
            else
                return mapHeight;                           // better use .map surface height
        }
        else
            return vmapHeight;                              // we have only vmapHeight (if have)
    }

    return mapHeight;                               // explicitly use map data
}

inline bool IsOutdoorWMO(uint32 mogpFlags, int32 /*adtId*/, int32 /*rootId*/, int32 /*groupId*/, WMOAreaTableEntry const* wmoEntry, AreaTableEntry const* atEntry)
{
    bool outdoor = true;

    if (wmoEntry && atEntry)
    {
        if (atEntry->flags & AREA_FLAG_OUTSIDE)
            return true;
        if (atEntry->flags & AREA_FLAG_INSIDE)
            return false;
    }

    outdoor = mogpFlags&0x8;

    if (wmoEntry)
    {
        if (wmoEntry->Flags & 4)
            return true;
        if ((wmoEntry->Flags & 2)!=0)
            outdoor = false;
    }
    return outdoor;
}

bool Map::IsOutdoors(float x, float y, float z) const
{
    uint32 mogpFlags;
    int32 adtId, rootId, groupId;

    // no wmo found? -> outside by default
    if (!GetAreaInfo(x, y, z, mogpFlags, adtId, rootId, groupId))
        return true;

    AreaTableEntry const* atEntry = 0;
    WMOAreaTableEntry const* wmoEntry= GetWMOAreaTableEntryByTripple(rootId, adtId, groupId);
    if (wmoEntry)
    {
        TC_LOG_DEBUG("maps", "Got WMOAreaTableEntry! flag %u, areaid %u", wmoEntry->Flags, wmoEntry->areaId);
        atEntry = GetAreaEntryByAreaID(wmoEntry->areaId);
    }
    return IsOutdoorWMO(mogpFlags, adtId, rootId, groupId, wmoEntry, atEntry);
}

bool Map::GetAreaInfo(float x, float y, float z, uint32 &flags, int32 &adtId, int32 &rootId, int32 &groupId) const
{
    float vmap_z = z;
    VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
    if (vmgr->getAreaInfo(GetId(), x, y, vmap_z, flags, adtId, rootId, groupId))
    {
        // check if there's terrain between player height and object height
        if (GridMap* gmap = const_cast<Map*>(this)->GetGrid(x, y))
        {
            float _mapheight = gmap->getHeight(x, y);
            // z + 2.0f condition taken from GetHeight(), not sure if it's such a great choice...
            if (z + 2.0f > _mapheight &&  _mapheight > vmap_z)
                return false;
        }
        return true;
    }
    return false;
}

uint16 Map::GetAreaFlag(float x, float y, float z, bool *isOutdoors) const
{
    uint32 mogpFlags;
    int32 adtId, rootId, groupId;
    WMOAreaTableEntry const* wmoEntry = 0;
    AreaTableEntry const* atEntry = 0;
    bool haveAreaInfo = false;

    if (GetAreaInfo(x, y, z, mogpFlags, adtId, rootId, groupId))
    {
        haveAreaInfo = true;
        wmoEntry = GetWMOAreaTableEntryByTripple(rootId, adtId, groupId);
        if (wmoEntry)
            atEntry = GetAreaEntryByAreaID(wmoEntry->areaId);
    }

    uint16 areaflag;

    if (atEntry)
        areaflag = atEntry->exploreFlag;
    else
    {
        if (GridMap* gmap = const_cast<Map*>(this)->GetGrid(x, y))
            areaflag = gmap->getArea(x, y);
        // this used while not all *.map files generated (instances)
        else
            areaflag = GetAreaFlagByMapId(i_mapEntry->MapID);
    }

    if (isOutdoors)
    {
        if (haveAreaInfo)
            *isOutdoors = IsOutdoorWMO(mogpFlags, adtId, rootId, groupId, wmoEntry, atEntry);
        else
            *isOutdoors = true;
    }
    return areaflag;
 }

uint8 Map::GetTerrainType(float x, float y) const
{
    if (GridMap* gmap = const_cast<Map*>(this)->GetGrid(x, y))
        return gmap->getTerrainType(x, y);
    else
        return 0;
}

ZLiquidStatus Map::getLiquidStatus(float x, float y, float z, uint8 ReqLiquidType, LiquidData* data) const
{
    ZLiquidStatus result = LIQUID_MAP_NO_WATER;
    VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
    float liquid_level = INVALID_HEIGHT;
    float ground_level = INVALID_HEIGHT;
    uint32 liquid_type = 0;
    if (vmgr->GetLiquidLevel(GetId(), x, y, z, ReqLiquidType, liquid_level, ground_level, liquid_type))
    {
        TC_LOG_DEBUG("maps", "getLiquidStatus(): vmap liquid level: %f ground: %f type: %u", liquid_level, ground_level, liquid_type);
        // Check water level and ground level
        if (liquid_level > ground_level && z > ground_level - 2)
        {
            // All ok in water -> store data
            if (data)
            {
                // hardcoded in client like this
                if (GetId() == 530 && liquid_type == 2)
                    liquid_type = 15;

                uint32 liquidFlagType = 0;
                if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(liquid_type))
                    liquidFlagType = liq->Type;

                if (liquid_type && liquid_type < 21)
                {
                    if (AreaTableEntry const* area = GetAreaEntryByAreaFlagAndMap(GetAreaFlag(x, y, z), GetId()))
                    {
                        uint32 overrideLiquid = area->LiquidTypeOverride[liquidFlagType];
                        if (!overrideLiquid && area->zone)
                        {
                            area = GetAreaEntryByAreaID(area->zone);
                            if (area)
                                overrideLiquid = area->LiquidTypeOverride[liquidFlagType];
                        }

                        if (LiquidTypeEntry const* liq = sLiquidTypeStore.LookupEntry(overrideLiquid))
                        {
                            liquid_type = overrideLiquid;
                            liquidFlagType = liq->Type;
                        }
                    }
                }

                data->level = liquid_level;
                data->depth_level = ground_level;

                data->entry = liquid_type;
                data->type_flags = 1 << liquidFlagType;
            }

            float delta = liquid_level - z;

            // Get position delta
            if (delta > 2.0f)                   // Under water
                return LIQUID_MAP_UNDER_WATER;
            if (delta > 0.0f)                   // In water
                return LIQUID_MAP_IN_WATER;
            if (delta > -0.1f)                   // Walk on water
                return LIQUID_MAP_WATER_WALK;
            result = LIQUID_MAP_ABOVE_WATER;
        }
    }

    if (GridMap* gmap = const_cast<Map*>(this)->GetGrid(x, y))
    {
        LiquidData map_data;
        ZLiquidStatus map_result = gmap->getLiquidStatus(x, y, z, ReqLiquidType, &map_data);
        // Not override LIQUID_MAP_ABOVE_WATER with LIQUID_MAP_NO_WATER:
        if (map_result != LIQUID_MAP_NO_WATER && (map_data.level > ground_level))
        {
            if (data)
            {
                // hardcoded in client like this
                if (GetId() == 530 && map_data.entry == 2)
                    map_data.entry = 15;

                *data = map_data;
            }
            return map_result;
        }
    }
    return result;
}

float Map::GetWaterLevel(float x, float y) const
{
    if (GridMap* gmap = const_cast<Map*>(this)->GetGrid(x, y))
        return gmap->getLiquidLevel(x, y);
    else
        return 0;
}

uint32 Map::GetAreaIdByAreaFlag(uint16 areaflag, uint32 map_id)
{
    AreaTableEntry const* entry = GetAreaEntryByAreaFlagAndMap(areaflag, map_id);

    if (entry)
        return entry->ID;
    else
        return 0;
}

uint32 Map::GetZoneIdByAreaFlag(uint16 areaflag, uint32 map_id)
{
    AreaTableEntry const* entry = GetAreaEntryByAreaFlagAndMap(areaflag, map_id);

    if (entry)
        return (entry->zone != 0) ? entry->zone : entry->ID;
    else
        return 0;
}

void Map::GetZoneAndAreaIdByAreaFlag(uint32& zoneid, uint32& areaid, uint16 areaflag, uint32 map_id)
{
    AreaTableEntry const* entry = GetAreaEntryByAreaFlagAndMap(areaflag, map_id);

    areaid = entry ? entry->ID : 0;
    zoneid = entry ? ((entry->zone != 0) ? entry->zone : entry->ID) : 0;
}

bool Map::isInLineOfSight(float x1, float y1, float z1, float x2, float y2, float z2, uint32 phasemask) const
{
    return VMAP::VMapFactory::createOrGetVMapManager()->isInLineOfSight(GetId(), x1, y1, z1, x2, y2, z2)
        && _dynamicTree.isInLineOfSight(x1, y1, z1, x2, y2, z2, phasemask);
}

bool Map::getObjectHitPos(uint32 phasemask, float x1, float y1, float z1, float x2, float y2, float z2, float& rx, float& ry, float& rz, float modifyDist)
{
    G3D::Vector3 startPos = G3D::Vector3(x1, y1, z1);
    G3D::Vector3 dstPos = G3D::Vector3(x2, y2, z2);

    G3D::Vector3 resultPos;
    bool result = _dynamicTree.getObjectHitPos(phasemask, startPos, dstPos, resultPos, modifyDist);

    rx = resultPos.x;
    ry = resultPos.y;
    rz = resultPos.z;
    return result;
}

float Map::GetHeight(uint32 phasemask, float x, float y, float z, bool vmap/*=true*/, float maxSearchDist/*=DEFAULT_HEIGHT_SEARCH*/) const
{
    return std::max<float>(GetHeight(x, y, z + 0.5f, vmap, maxSearchDist), _dynamicTree.getHeight(x, y, z + 0.5f, maxSearchDist, phasemask));
}

bool Map::IsInWater(float x, float y, float pZ, LiquidData* data) const
{
    // Check surface in x, y point for liquid
    if (const_cast<Map*>(this)->GetGrid(x, y))
    {
        LiquidData liquid_status;
        LiquidData* liquid_ptr = data ? data : &liquid_status;
        if (getLiquidStatus(x, y, pZ, MAP_ALL_LIQUIDS, liquid_ptr))
            return true;
    }
    return false;
}

bool Map::IsUnderWater(float x, float y, float z) const
{
    if (const_cast<Map*>(this)->GetGrid(x, y))
    {
        if (getLiquidStatus(x, y, z, MAP_LIQUID_TYPE_WATER|MAP_LIQUID_TYPE_OCEAN)&LIQUID_MAP_UNDER_WATER)
            return true;
    }
    return false;
}

char const* Map::GetMapName() const
{
    return i_mapEntry ? i_mapEntry->name : "UNNAMEDMAP\x0";
}

void Map::UpdateObjectVisibility(WorldObject* obj, Cell cell, CellCoord cellpair)
{
    cell.SetNoCreate();
    Trinity::VisibleChangesNotifier notifier(*obj);
    cell.Visit(cellpair, Trinity::makeWorldVisitor(notifier), *this, *obj, obj->GetVisibilityRange());
}

void Map::UpdateObjectsVisibilityFor(Player* player, Cell cell, CellCoord cellpair)
{
    Trinity::VisibleNotifier notifier(*player);

    cell.SetNoCreate();
    cell.Visit(cellpair, Trinity::makeWorldVisitor(notifier), *this, *player, player->GetSightRange());
    cell.Visit(cellpair, Trinity::makeGridVisitor(notifier), *this, *player, player->GetSightRange());

    // send data
    notifier.SendToSelf();
}

void Map::SendInitSelf(Player* player)
{
    TC_LOG_INFO("maps", "Creating player data for himself %u", player->GetGUIDLow());

    UpdateData data(player->GetMapId());

    // attach to player data current transport data
    if (Transport* transport = player->GetTransport())
    {
        transport->BuildCreateUpdateBlockForPlayer(&data, player);
    }

    // build data for self presence in world at own client (one time for map)
    player->BuildCreateUpdateBlockForPlayer(&data, player);

    // build other passengers at transport also (they always visible and marked as visible and will not send at visibility update at add to map
    if (Transport* transport = player->GetTransport())
    {
        for (Transport::PlayerSet::const_iterator itr = transport->GetPassengers().begin(); itr != transport->GetPassengers().end(); ++itr)
        {
            if (player != (*itr) && player->HaveAtClient(*itr))
            {
                (*itr)->BuildCreateUpdateBlockForPlayer(&data, player);
            }
        }
    }

    WorldPacket packet;
    if (data.BuildPacket(&packet))
        player->GetSession()->SendPacket(&packet);
}

void Map::SendInitTransports(Player* player)
{
    // Hack to send out transports
    MapManager::TransportMap& tmap = sMapMgr->m_TransportsByMap;

    // no transports at map
    if (tmap.find(player->GetMapId()) == tmap.end())
        return;

    UpdateData transData(player->GetMapId());

    MapManager::TransportSet& tset = tmap[player->GetMapId()];

    for (MapManager::TransportSet::const_iterator i = tset.begin(); i != tset.end(); ++i)
    {
        // send data for current transport in other place
        if ((*i) != player->GetTransport() && (*i)->GetMapId() == GetId())
        {
            (*i)->BuildCreateUpdateBlockForPlayer(&transData, player);
        }
    }

    WorldPacket packet;
    if (transData.BuildPacket(&packet))
        player->GetSession()->SendPacket(&packet);
}

void Map::SendRemoveTransports(Player* player)
{
    // Hack to send out transports
    MapManager::TransportMap& tmap = sMapMgr->m_TransportsByMap;

    // no transports at map
    if (tmap.find(player->GetMapId()) == tmap.end())
        return;

    UpdateData transData(player->GetMapId());

    MapManager::TransportSet& tset = tmap[player->GetMapId()];

    // except used transport
    for (MapManager::TransportSet::const_iterator i = tset.begin(); i != tset.end(); ++i)
        if ((*i) != player->GetTransport() && (*i)->GetMapId() != GetId())
            (*i)->BuildOutOfRangeUpdateBlock(&transData);

    WorldPacket packet;
    if (transData.BuildPacket(&packet))
        player->GetSession()->SendPacket(&packet);
}

void Map::setNGrid(NGrid *grid, uint32 x, uint32 y)
{
    if (x >= MAX_NUMBER_OF_GRIDS || y >= MAX_NUMBER_OF_GRIDS)
    {
        TC_LOG_ERROR("maps", "map::setNGrid() Invalid grid coordinates found: %d, %d!", x, y);
        ASSERT(false);
    }
    i_grids[x][y] = grid;
}

void Map::DelayedUpdate(const uint32 diff)
{
    RemoveAllObjectsInRemoveList();

    // Don't unload grids if it's battleground, since we may have manually added GOs, creatures, those doesn't load from DB at grid re-load !
    // This isn't really bother us, since as soon as we have instanced BG-s, the whole map unloads as the BG gets ended
    if (!IsBattlegroundOrArena())
    {
        for (auto i = i_loadedGrids.begin(); i != i_loadedGrids.end();)
        {
            auto const state = i->second.GetGridState();
            si_GridStates[state](*this, i++, diff);
        }
    }
}

void Map::AddObjectToRemoveList(WorldObject* obj)
{
    ASSERT(obj->GetMapId() == GetId() && obj->GetInstanceId() == GetInstanceId());

    obj->CleanupsBeforeDelete(false);                            // remove or simplify at least cross referenced links

    i_objectsToRemove.insert(obj);
    //TC_LOG_DEBUG("maps", "Object (GUID: %u TypeId: %u) added to removing list.", obj->GetGUIDLow(), obj->GetTypeId());
}

void Map::AddObjectToSwitchList(WorldObject* obj, bool on)
{
    ASSERT(obj->GetMapId() == GetId() && obj->GetInstanceId() == GetInstanceId());
    // i_objectsToSwitch is iterated only in Map::RemoveAllObjectsInRemoveList() and it uses
    // the contained objects only if GetTypeId() == TYPEID_UNIT , so we can return in all other cases
    if (obj->GetTypeId() != TYPEID_UNIT)
        return;

    std::map<WorldObject*, bool>::iterator itr = i_objectsToSwitch.find(obj);
    if (itr == i_objectsToSwitch.end())
        i_objectsToSwitch.insert(itr, std::make_pair(obj, on));
    else if (itr->second != on)
        i_objectsToSwitch.erase(itr);
    else
        ASSERT(false);
}

void Map::RemoveAllObjectsInRemoveList()
{
    while (!i_objectsToSwitch.empty())
    {
        std::map<WorldObject*, bool>::iterator itr = i_objectsToSwitch.begin();
        WorldObject* obj = itr->first;
        bool on = itr->second;
        i_objectsToSwitch.erase(itr);

        if (obj->GetTypeId() == TYPEID_UNIT && !obj->IsPermanentWorldObject())
            SwitchGridContainers(obj->ToCreature(), on);
    }

    //TC_LOG_DEBUG("maps", "Object remover 1 check.");
    while (!i_objectsToRemove.empty())
    {
        std::set<WorldObject*>::iterator itr = i_objectsToRemove.begin();
        WorldObject* obj = *itr;

        switch (obj->GetTypeId())
        {
            case TYPEID_CORPSE:
            {
                Corpse* corpse = ObjectAccessor::GetCorpse(*obj, obj->GetGUID());
                if (!corpse)
                    TC_LOG_ERROR("maps", "Tried to delete corpse/bones %u that is not in map.", obj->GetGUIDLow());
                else
                    RemoveFromMap(corpse, true);
                break;
            }
        case TYPEID_DYNAMICOBJECT:
            RemoveFromMap((DynamicObject*)obj, true);
            break;
        case TYPEID_AREATRIGGER:
            RemoveFromMap((AreaTrigger*)obj, true);
            break;
        case TYPEID_GAMEOBJECT:
            RemoveFromMap((GameObject*)obj, true);
            break;
        case TYPEID_UNIT:
            // in case triggered sequence some spell can continue casting after prev CleanupsBeforeDelete call
            // make sure that like sources auras/etc removed before destructor start
            obj->ToCreature()->CleanupsBeforeDelete();
            RemoveFromMap(obj->ToCreature(), true);
            break;
        default:
            TC_LOG_ERROR("maps", "Non-grid object (TypeId: %u) is in grid object remove list, ignored.", obj->GetTypeId());
            break;
        }

        i_objectsToRemove.erase(itr);
    }

    //TC_LOG_DEBUG("maps", "Object remover 2 check.");
}

uint32 Map::GetPlayersCountExceptGMs() const
{
    uint32 count = 0;
    for (MapRefManager::const_iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        if (!itr->GetSource()->isGameMaster())
            ++count;
    return count;
}

void Map::SendToPlayers(WorldPacket const* data) const
{
    for (MapRefManager::const_iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        itr->GetSource()->GetSession()->SendPacket(data);
}

bool Map::ActiveObjectsNearGrid(NGrid const &ngrid) const
{
    CellCoord cell_min(ngrid.getX() * MAX_NUMBER_OF_CELLS, ngrid.getY() * MAX_NUMBER_OF_CELLS);
    CellCoord cell_max(cell_min.x_coord + MAX_NUMBER_OF_CELLS, cell_min.y_coord+MAX_NUMBER_OF_CELLS);

    //we must find visible range in cells so we unload only non-visible cells...
    float viewDist = GetVisibilityRange();
    int cell_range = (int)ceilf(viewDist / SIZE_OF_GRID_CELL) + 1;

    cell_min.dec_x(cell_range);
    cell_min.dec_y(cell_range);
    cell_max.inc_x(cell_range);
    cell_max.inc_y(cell_range);

    for (MapRefManager::const_iterator iter = m_mapRefManager.begin(); iter != m_mapRefManager.end(); ++iter)
    {
        Player const * const player = iter->GetSource();

        auto const p = Trinity::ComputeCellCoord(player->GetPositionX(), player->GetPositionY());
        if ((cell_min.x_coord <= p.x_coord && p.x_coord <= cell_max.x_coord)
                && (cell_min.y_coord <= p.y_coord && p.y_coord <= cell_max.y_coord))
        {
            return true;
        }
    }

    for (auto const &obj : m_activeNonPlayers)
    {
        auto const p = Trinity::ComputeCellCoord(obj->GetPositionX(), obj->GetPositionY());
        if ((cell_min.x_coord <= p.x_coord && p.x_coord <= cell_max.x_coord)
                && (cell_min.y_coord <= p.y_coord && p.y_coord <= cell_max.y_coord))
        {
            return true;
        }
    }

    return false;
}

void Map::AddToActive(Creature* c)
{
    AddToActiveHelper(c);

    // also not allow unloading spawn grid to prevent creating creature clone at load
    if (!c->isPet() && c->GetDBTableGUIDLow())
    {
        float x, y, z;
        c->GetRespawnPosition(x, y, z);
        GridCoord p = Trinity::ComputeGridCoord(x, y);
        if (getNGrid(p.x_coord, p.y_coord))
            getNGrid(p.x_coord, p.y_coord)->incUnloadActiveLock();
        else
        {
            GridCoord p2 = Trinity::ComputeGridCoord(c->GetPositionX(), c->GetPositionY());
            TC_LOG_ERROR("maps", "Active creature (GUID: %u Entry: %u) added to grid[%u, %u] but spawn grid[%u, %u] was not loaded.",
                c->GetGUIDLow(), c->GetEntry(), p.x_coord, p.y_coord, p2.x_coord, p2.y_coord);
        }
    }
}

void Map::RemoveFromActive(Creature* c)
{
    RemoveFromActiveHelper(c);

    // also allow unloading spawn grid
    if (!c->isPet() && c->GetDBTableGUIDLow())
    {
        float x, y, z;
        c->GetRespawnPosition(x, y, z);
        GridCoord p = Trinity::ComputeGridCoord(x, y);
        if (getNGrid(p.x_coord, p.y_coord))
            getNGrid(p.x_coord, p.y_coord)->decUnloadActiveLock();
        else
        {
            GridCoord p2 = Trinity::ComputeGridCoord(c->GetPositionX(), c->GetPositionY());
            TC_LOG_ERROR("maps", "Active creature (GUID: %u Entry: %u) removed from grid[%u, %u] but spawn grid[%u, %u] was not loaded.",
                c->GetGUIDLow(), c->GetEntry(), p.x_coord, p.y_coord, p2.x_coord, p2.y_coord);
        }
    }
}

template bool Map::AddToMap(Corpse*);
template bool Map::AddToMap(Creature*);
template bool Map::AddToMap(GameObject*);
template bool Map::AddToMap(DynamicObject*);
template bool Map::AddToMap(AreaTrigger*);

template void Map::RemoveFromMap(Corpse*, bool);
template void Map::RemoveFromMap(Creature*, bool);
template void Map::RemoveFromMap(GameObject*, bool);
template void Map::RemoveFromMap(DynamicObject*, bool);
template void Map::RemoveFromMap(AreaTrigger*, bool);

/* ******* Dungeon Instance Maps ******* */

InstanceMap::InstanceMap(uint32 id, time_t expiry, uint32 InstanceId, uint8 SpawnMode, Map* _parent)
  : Map(id, expiry, InstanceId, SpawnMode, _parent),
    m_resetAfterUnload(false), m_unloadWhenEmpty(false),
    i_data(NULL), i_script_id(0)
{
    //lets initialize visibility distance for dungeons
    InstanceMap::InitVisibilityDistance();

    // the timer is started by default, and stopped when the first player joins
    // this make sure it gets unloaded if for some reason no player joins
    m_unloadTimer = std::max(sWorld->getIntConfig(CONFIG_INSTANCE_UNLOAD_DELAY), (uint32)MIN_UNLOAD_DELAY);
}

InstanceMap::~InstanceMap()
{
    delete i_data;
    i_data = NULL;
}

void InstanceMap::InitVisibilityDistance()
{
    //init visibility distance for instances
    m_VisibleDistance = sWorld->GetMaxVisibleDistanceInInstances();
    m_VisibilityNotifyPeriod = sWorld->GetVisibilityNotifyPeriodInInstances();
}

/*
    Do map specific checks to see if the player can enter
*/
bool InstanceMap::CanEnter(Player* player)
{
    if (player->GetMapRef().getTarget() == this)
    {
        TC_LOG_ERROR("maps", "InstanceMap::CanEnter - player %s(%u) already in map %d, %d, %d!",
                     player->GetName().c_str(), player->GetGUIDLow(), GetId(), GetInstanceId(), GetSpawnMode());
        ASSERT(false);
        return false;
    }

    // allow GM's to enter
    if (player->isGameMaster())
        return Map::CanEnter(player);

    // cannot enter if the instance is full (player cap), GMs don't count
    uint32 maxPlayers = GetMaxPlayers();
    if (GetPlayersCountExceptGMs() >= maxPlayers)
    {
        TC_LOG_INFO("maps", "MAP: Instance '%u' of map '%s' cannot have more than '%u' players. Player '%s' rejected",
                    GetInstanceId(), GetMapName(), maxPlayers, player->GetName().c_str());
        player->SendTransferAborted(GetId(), TRANSFER_ABORT_MAX_PLAYERS);
        return false;
    }

    // cannot enter while an encounter is in progress on raids
    /*Group* group = player->GetGroup();
    if (!player->isGameMaster() && group && group->InCombatToInstance(GetInstanceId()) && player->GetMapId() != GetId())*/
    if (IsRaid() && GetInstanceScript() && GetInstanceScript()->IsEncounterInProgress())
    {
        player->SendTransferAborted(GetId(), TRANSFER_ABORT_ZONE_IN_COMBAT);
        return false;
    }

    // cannot enter if instance is in use by another party/soloer that have a
    // permanent save in the same instance id

    PlayerList const &playerList = GetPlayers();

    if (!playerList.isEmpty())
        for (PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player* iPlayer = i->GetSource())
            {
                if (iPlayer->isGameMaster()) // bypass GMs
                    continue;
                if (!player->GetGroup()) // player has not group and there is someone inside, deny entry
                {
                    player->SendTransferAborted(GetId(), TRANSFER_ABORT_MAX_PLAYERS);
                    return false;
                }
                // player inside instance has no group or his groups is different to entering player's one, deny entry
                if (!iPlayer->GetGroup() || iPlayer->GetGroup() != player->GetGroup())
                {
                    player->SendTransferAborted(GetId(), TRANSFER_ABORT_MAX_PLAYERS);
                    return false;
                }
                break;
            }

    return Map::CanEnter(player);
}

/*
    Do map specific checks and add the player to the map if successful.
*/
bool InstanceMap::AddPlayerToMap(Player* player)
{
    // TODO: Not sure about checking player level: already done in HandleAreaTriggerOpcode
    // GMs still can teleport player in instance.
    // Is it needed?

    // Check moved to void WorldSession::HandleMoveWorldportAckOpcode()
    //if (!CanEnter(player))
        //return false;

    // Dungeon only code
    if (IsDungeon())
    {
        Group* group = player->GetGroup();

        // increase current instances (hourly limit)
        if (!group || !group->isLFGGroup())
            player->AddInstanceEnterTime(GetInstanceId(), time(NULL));

        // get or create an instance save for the map
        InstanceSave* mapSave = sInstanceSaveMgr->GetInstanceSave(GetInstanceId());
        if (!mapSave)
        {
            TC_LOG_INFO("maps", "InstanceMap::Add: creating instance save for map %d spawnmode %d with instance id %d", GetId(), GetSpawnMode(), GetInstanceId());
            mapSave = sInstanceSaveMgr->AddInstanceSave(GetId(), GetInstanceId(), Difficulty(GetSpawnMode()), 0, true);
        }

        // check for existing instance binds
        InstancePlayerBind* playerBind = player->GetBoundInstance(GetId(), Difficulty(GetSpawnMode()));
        if (playerBind && playerBind->perm)
        {
            // cannot enter other instances if bound permanently
            if (playerBind->save != mapSave)
            {
                TC_LOG_ERROR("maps", "InstanceMap::Add: player %s(%d) is permanently bound to instance %d, %d, %d, %d, %d, %d"
                             " but he is being put into instance %d, %d, %d, %d, %d, %d",
                             player->GetName().c_str(), player->GetGUIDLow(), playerBind->save->GetMapId(),
                             playerBind->save->GetInstanceId(), playerBind->save->GetDifficulty(),
                             playerBind->save->GetPlayerCount(), playerBind->save->GetGroupCount(),
                             playerBind->save->CanReset(), mapSave->GetMapId(), mapSave->GetInstanceId(),
                             mapSave->GetDifficulty(), mapSave->GetPlayerCount(), mapSave->GetGroupCount(), mapSave->CanReset());
                return false;
            }
        }
        else
        {
            if (group)
            {
                // solo saves should be reset when entering a group
                InstanceGroupBind* groupBind = group->GetBoundInstance(this);
                if (playerBind && playerBind->save != mapSave)
                {
                    TC_LOG_ERROR("maps", "InstanceMap::Add: player %s(%d) is being put into instance %d, %d, %d, %d, %d, %d"
                                 " but he is in group %d and is bound to instance %d, %d, %d, %d, %d, %d!",
                                 player->GetName().c_str(), player->GetGUIDLow(), mapSave->GetMapId(), mapSave->GetInstanceId(),
                                 mapSave->GetDifficulty(), mapSave->GetPlayerCount(), mapSave->GetGroupCount(), mapSave->CanReset(),
                                 GUID_LOPART(group->GetLeaderGUID()), playerBind->save->GetMapId(), playerBind->save->GetInstanceId(),
                                 playerBind->save->GetDifficulty(), playerBind->save->GetPlayerCount(), playerBind->save->GetGroupCount(),
                                 playerBind->save->CanReset());

                    if (groupBind)
                    {
                        TC_LOG_ERROR("maps", "InstanceMap::Add: the group is bound to the instance %d, %d, %d, %d, %d, %d",
                                     groupBind->save->GetMapId(), groupBind->save->GetInstanceId(), groupBind->save->GetDifficulty(),
                                     groupBind->save->GetPlayerCount(), groupBind->save->GetGroupCount(), groupBind->save->CanReset());
                    }

                    //ASSERT(false);
                    return false;
                }
                // bind to the group or keep using the group save
                if (!groupBind)
                    group->BindToInstance(mapSave, false);
                else
                {
                    // cannot jump to a different instance without resetting it
                    if (groupBind->save != mapSave)
                    {
                        TC_LOG_ERROR("maps", "InstanceMap::Add: player %s(%d) is being put into instance %d, %d, %d but he is in"
                                     " group %d which is bound to instance %d, %d, %d!",
                                     player->GetName().c_str(), player->GetGUIDLow(), mapSave->GetMapId(), mapSave->GetInstanceId(),
                                     mapSave->GetDifficulty(), GUID_LOPART(group->GetLeaderGUID()), groupBind->save->GetMapId(),
                                     groupBind->save->GetInstanceId(), groupBind->save->GetDifficulty());

                        if (mapSave)
                            TC_LOG_ERROR("maps", "MapSave players: %d, group count: %d", mapSave->GetPlayerCount(), mapSave->GetGroupCount());
                        else
                            TC_LOG_ERROR("maps", "MapSave NULL");
                        if (groupBind->save)
                            TC_LOG_ERROR("maps", "GroupBind save players: %d, group count: %d", groupBind->save->GetPlayerCount(), groupBind->save->GetGroupCount());
                        else
                            TC_LOG_ERROR("maps", "GroupBind save NULL");
                        return false;
                    }
                    // if the group/leader is permanently bound to the instance
                    // players also become permanently bound when they enter
                    if (groupBind->perm)
                    {
                        WorldPacket data(SMSG_INSTANCE_LOCK_WARNING_QUERY, 4 + 4 + 1);
                        data << uint32(60000);
                        data << uint32(i_data ? i_data->GetCompletedEncounterMask() : 0);

                        data.WriteBit(0);               // WarningOnly
                        data.WriteBit(0);               // Extending
                        data.FlushBits();

                        player->GetSession()->SendPacket(&data);
                        player->SetPendingBind(mapSave->GetInstanceId(), 60000);
                    }
                }
            }
            else
            {
                // set up a solo bind or continue using it
                if (!playerBind)
                    player->BindToInstance(mapSave, false);
                else
                    if (playerBind->save != mapSave)
                        return false;
                    //ASSERT(playerBind->save == mapSave);
            }
        }

        if (group && group->isLFGGroup())
            if (uint32 dungeonId = sLFGMgr->GetDungeon(group->GetGUID(), true))
                if (LFGDungeonEntry const* dungeon = sLFGDungeonStore.LookupEntry(dungeonId))
                    if (LFGDungeonEntry const* randomDungeon = sLFGDungeonStore.LookupEntry(*(sLFGMgr->GetSelectedDungeons(player->GetGUID()).begin())))
                        if (uint32(dungeon->map) == GetId() && dungeon->difficulty == uint32(GetDifficulty()) && randomDungeon->type == uint32(TYPEID_RANDOM_DUNGEON))
                            player->CastSpell(player, LFG_SPELL_LUCK_OF_THE_DRAW, true);
    }

    // for normal instances cancel the reset schedule when the
    // first player enters (no players yet)
    SetResetSchedule(false);

    TC_LOG_INFO("maps", "MAP: Player '%s' entered instance '%u' of map '%s'", player->GetName().c_str(), GetInstanceId(), GetMapName());
    // initialize unload state
    m_unloadTimer = 0;
    m_resetAfterUnload = false;
    m_unloadWhenEmpty = false;

    // this will acquire the same mutex so it cannot be in the previous block
    Map::AddPlayerToMap(player);

    if (i_data)
        i_data->OnPlayerEnter(player);

    return true;
}

void InstanceMap::Update(const uint32 t_diff)
{
    Map::Update(t_diff);

    if (i_data)
        i_data->Update(t_diff);
}

void InstanceMap::RemovePlayerFromMap(Player* player, bool remove)
{
    TC_LOG_INFO("maps", "MAP: Removing player '%s' from instance '%u' of map '%s' before relocating to another map",
                player->GetName().c_str(), GetInstanceId(), GetMapName());

    //if last player set unload timer
    if (!m_unloadTimer && m_mapRefManager.getSize() == 1)
        m_unloadTimer = m_unloadWhenEmpty ? MIN_UNLOAD_DELAY : std::max(sWorld->getIntConfig(CONFIG_INSTANCE_UNLOAD_DELAY), (uint32)MIN_UNLOAD_DELAY);
    Map::RemovePlayerFromMap(player, remove);

    if (i_data)
        i_data->OnPlayerLeave(player);

    // for normal instances schedule the reset after all players have left
    SetResetSchedule(true);
}

void InstanceMap::CreateInstanceData(bool load)
{
    if (i_data != NULL)
        return;

    InstanceTemplate const* mInstance = sObjectMgr->GetInstanceTemplate(GetId());
    if (mInstance)
    {
        i_script_id = mInstance->ScriptId;
        i_data = sScriptMgr->CreateInstanceData(this);
    }

    if (!i_data)
        return;

    i_data->Initialize();

    if (load)
    {
        // TODO: make a global storage for this
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_INSTANCE);
        stmt->setUInt16(0, uint16(GetId()));
        stmt->setUInt32(1, i_InstanceId);
        PreparedQueryResult result = CharacterDatabase.Query(stmt);

        if (result)
        {
            Field* fields = result->Fetch();
            std::string data = fields[0].GetString();
            i_data->SetCompletedEncountersMask(fields[1].GetUInt32());
            if (data != "")
            {
                TC_LOG_DEBUG("maps", "Loading instance data for `%s` with id %u", sObjectMgr->GetScriptName(i_script_id), i_InstanceId);
                i_data->Load(data.c_str());
            }
        }
    }
}

/*
    Returns true if there are no players in the instance
*/
bool InstanceMap::Reset(uint8 method)
{
    // note: since the map may not be loaded when the instance needs to be reset
    // the instance must be deleted from the DB by InstanceSaveManager

    if (HavePlayers())
    {
        if (method == INSTANCE_RESET_ALL || method == INSTANCE_RESET_CHANGE_DIFFICULTY)
        {
            // notify the players to leave the instance so it can be reset
            for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
                itr->GetSource()->SendResetFailedNotify(GetId());
        }
        else
        {
            if (method == INSTANCE_RESET_GLOBAL)
                // set the homebind timer for players inside (1 minute)
                for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
                    itr->GetSource()->m_InstanceValid = false;

            // the unload timer is not started
            // instead the map will unload immediately after the players have left
            m_unloadWhenEmpty = true;
            m_resetAfterUnload = true;
        }
    }
    else
    {
        // unloaded at next update
        m_unloadTimer = MIN_UNLOAD_DELAY;
        m_resetAfterUnload = true;
    }

    return m_mapRefManager.isEmpty();
}

void InstanceMap::PermBindAllPlayers(Player* source)
{
    if (Group* group = source->GetGroup())
        if (uint32 entry = source->GetLastKilledCreature())
            group->SetLastBind(entry);

    if (!IsDungeon() || IsLFR())
        return;

    InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(GetInstanceId());

    if (!save) // Group disbanded just before killing the boss... create a new save
        save = sInstanceSaveMgr->AddInstanceSave(GetId(), GetInstanceId(), GetDifficulty(), 0, true);

    std::map<uint64, Player*> players;

    // Bind group to instance even if leader is outside
    if (Group* group = source->GetGroup())
    {
        group->BindToInstance(save, true);

        // Add all group members that have enter the instance (even before the fight began and
        // currently outside. Fast check: See if the instance is in the list of last 5 entered
        for (GroupReference* itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
            if (Player* player = itr->GetSource())
                if (player->HasEnterInstance(GetInstanceId()))
                    players[player->GetGUID()] = player;
    }

    // Add all players inside (even if they are not in the same group)
    for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        if (Player* player = itr->GetSource())
            players[player->GetGUID()] = player;


    // Now save all players to the instance
    for (auto itr = players.begin(); itr != players.end(); ++itr)
    {
        Player* player = itr->second;
        // players inside an instance cannot be bound to other instances
        // some players may already be permanently bound, in this case nothing happens
        InstancePlayerBind* bind = player->GetBoundInstance(save->GetMapId(), save->GetDifficulty());
        if (!bind || !bind->perm)
        {
            player->BindToInstance(save, true);

            WorldPacket data(SMSG_INSTANCE_SAVE_CREATED, 1);
            data.WriteBit(0);               // GM
            data.FlushBits();
            player->SendDirectMessage(&data);

            player->GetSession()->SendCalendarRaidLockout(save, true);
        }
    }
}

void InstanceMap::UnloadAll()
{
    ASSERT(!HavePlayers());

    if (m_resetAfterUnload == true)
        DeleteRespawnTimes();

    Map::UnloadAll();
}

void InstanceMap::SendResetWarnings(uint32 timeLeft) const
{
    for (MapRefManager::const_iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        itr->GetSource()->SendInstanceResetWarning(GetId(), itr->GetSource()->GetDifficulty(IsRaid()), timeLeft);
}

void InstanceMap::SetResetSchedule(bool on)
{
    // only for normal instances
    // the reset time is only scheduled when there are no payers inside
    // it is assumed that the reset time will rarely (if ever) change while the reset is scheduled
    if (IsDungeon() && !HavePlayers() && !IsRaidOrHeroicDungeon())
    {
        if (InstanceSave* save = sInstanceSaveMgr->GetInstanceSave(GetInstanceId()))
            sInstanceSaveMgr->ScheduleReset(on, save->GetResetTime(), InstanceSaveManager::InstResetEvent(0, GetId(), Difficulty(GetSpawnMode()), GetInstanceId()));
        else
            TC_LOG_ERROR("maps", "InstanceMap::SetResetSchedule: cannot turn schedule %s, there is no save information for instance (map [id: %u, name: %s], instance id: %u, difficulty: %u)",
                on ? "on" : "off", GetId(), GetMapName(), GetInstanceId(), Difficulty(GetSpawnMode()));
    }
}

MapDifficulty const* Map::GetMapDifficulty() const
{
    return GetMapDifficultyData(GetId(), GetDifficulty());
}

uint32 InstanceMap::GetMaxPlayers() const
{
    if (MapDifficulty const* mapDiff = GetMapDifficulty())
    {
        if (mapDiff->maxPlayers || IsRegularDifficulty())    // Normal case (expect that regular difficulty always have correct maxplayers)
            return mapDiff->maxPlayers;
        else                                                // DBC have 0 maxplayers for heroic instances with expansion < 2
        {                                                   // The heroic entry exists, so we don't have to check anything, simply return normal max players
            MapDifficulty const* normalDiff = GetMapDifficultyData(GetId(), REGULAR_DIFFICULTY);
            return normalDiff ? normalDiff->maxPlayers : 0;
        }
    }
    else                                                    // I'd rather ASSERT(false);
    {
        switch (GetDifficulty())
        {
            case Difficulty::SCENARIO_DIFFICULTY:
            case Difficulty::SCENARIO_HEROIC_DIFFICULTY:
                return 3;
            case Difficulty::HEROIC_DIFFICULTY:
            case Difficulty::CHALLENGE_MODE_DIFFICULTY:
                return 5;
            case Difficulty::MAN10_DIFFICULTY:
            case Difficulty::MAN10_HEROIC_DIFFICULTY:
                return 10;
            case Difficulty::MAN25_DIFFICULTY:
            case Difficulty::MAN25_HEROIC_DIFFICULTY:
            case Difficulty::DYNAMIC_DIFFICULTY:
                return 25;
            default:
                break;
        }

        return 0;
    }
}

uint32 InstanceMap::GetMaxResetDelay() const
{
    MapDifficulty const* mapDiff = GetMapDifficulty();
    return mapDiff ? mapDiff->resetTime : 0;
}

/* ******* Battleground Instance Maps ******* */

BattlegroundMap::BattlegroundMap(uint32 id, time_t expiry, uint32 InstanceId, Map* _parent, uint8 spawnMode)
  : Map(id, expiry, InstanceId, spawnMode, _parent), m_bg(NULL) { }

BattlegroundMap::~BattlegroundMap()
{
    if (m_bg)
    {
        //unlink to prevent crash, always unlink all pointer reference before destruction
        m_bg->SetBgMap(NULL);
        m_bg = NULL;
    }
}

void BattlegroundMap::InitVisibilityDistance()
{
    m_VisibleDistance = GetEntry()->IsBattleArena()
            ? sWorld->GetMaxVisibleDistanceInArenas()
            : sWorld->GetMaxVisibleDistanceInBG();
    m_VisibilityNotifyPeriod = sWorld->GetVisibilityNotifyPeriodInBGArenas();
}

bool BattlegroundMap::CanEnter(Player* player)
{
    if (player->GetMapRef().getTarget() == this)
    {
        TC_LOG_ERROR("maps", "BGMap::CanEnter - player %u is already in map!", player->GetGUIDLow());
        ASSERT(false);
        return false;
    }

    if (player->GetBattlegroundId() != GetInstanceId())
        return false;

    // player number limit is checked in bgmgr, no need to do it here

    return Map::CanEnter(player);
}

bool BattlegroundMap::AddPlayerToMap(Player* player)
{
    //Check moved to void WorldSession::HandleMoveWorldportAckOpcode()
    //if (!CanEnter(player))
    //return false;
    // reset instance validity, battleground maps do not homebind
    player->m_InstanceValid = true;
    return Map::AddPlayerToMap(player);
}

void BattlegroundMap::RemovePlayerFromMap(Player* player, bool remove)
{
    TC_LOG_INFO("maps", "MAP: Removing player '%s' from bg '%u' of map '%s' before relocating to another map",
                player->GetName().c_str(), GetInstanceId(), GetMapName());
    Map::RemovePlayerFromMap(player, remove);
}

void BattlegroundMap::SetUnload()
{
    m_unloadTimer = MIN_UNLOAD_DELAY;
}

void BattlegroundMap::RemoveAllPlayers()
{
    if (HavePlayers())
        for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
            if (Player* player = itr->GetSource())
                if (!player->IsBeingTeleportedFar())
                    player->TeleportTo(player->GetBattlegroundEntryPoint());
}

Creature* Map::GetCreature(uint64 guid)
{
    return ObjectAccessor::GetObjectInMap(guid, this, (Creature*)NULL);
}

GameObject* Map::GetGameObject(uint64 guid)
{
    return ObjectAccessor::GetObjectInMap(guid, this, (GameObject*)NULL);
}

DynamicObject* Map::GetDynamicObject(uint64 guid)
{
    return ObjectAccessor::GetObjectInMap(guid, this, (DynamicObject*)NULL);
}

void Map::UpdateIteratorBack(Player* player)
{
    if (m_mapRefIter == player->GetMapRef())
        m_mapRefIter = m_mapRefIter->nocheck_prev();
}

void Map::SaveCreatureRespawnTime(uint32 dbGuid, time_t respawnTime)
{
    if (!respawnTime)
    {
        // Delete only
        RemoveCreatureRespawnTime(dbGuid);
        return;
    }

    _creatureRespawnTimes[dbGuid] = respawnTime;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_CREATURE_RESPAWN);
    stmt->setUInt32(0, dbGuid);
    stmt->setUInt32(1, uint32(respawnTime));
    stmt->setUInt16(2, GetId());
    stmt->setUInt32(3, GetInstanceId());
    CharacterDatabase.Execute(stmt);
}

void Map::RemoveCreatureRespawnTime(uint32 dbGuid)
{
    _creatureRespawnTimes.erase(dbGuid);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN);
    stmt->setUInt32(0, dbGuid);
    stmt->setUInt16(1, GetId());
    stmt->setUInt32(2, GetInstanceId());
    CharacterDatabase.Execute(stmt);
}

void Map::SaveGORespawnTime(uint32 dbGuid, time_t respawnTime)
{
    if (!respawnTime)
    {
        // Delete only
        RemoveGORespawnTime(dbGuid);
        return;
    }

    _goRespawnTimes[dbGuid] = respawnTime;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GO_RESPAWN);
    stmt->setUInt32(0, dbGuid);
    stmt->setUInt32(1, uint32(respawnTime));
    stmt->setUInt16(2, GetId());
    stmt->setUInt32(3, GetInstanceId());
    CharacterDatabase.Execute(stmt);
}

void Map::RemoveGORespawnTime(uint32 dbGuid)
{
    _goRespawnTimes.erase(dbGuid);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GO_RESPAWN);
    stmt->setUInt32(0, dbGuid);
    stmt->setUInt16(1, GetId());
    stmt->setUInt32(2, GetInstanceId());
    CharacterDatabase.Execute(stmt);
}

void Map::LoadRespawnTimes()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CREATURE_RESPAWNS);
    stmt->setUInt16(0, GetId());
    stmt->setUInt32(1, GetInstanceId());
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        do
        {
            Field* fields      = result->Fetch();
            uint32 loguid      = fields[0].GetUInt32();
            uint32 respawnTime = fields[1].GetUInt32();

            _creatureRespawnTimes[loguid] = time_t(respawnTime);
        }
        while (result->NextRow());
    }

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GO_RESPAWNS);
    stmt->setUInt16(0, GetId());
    stmt->setUInt32(1, GetInstanceId());
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
    {
        do
        {
            Field* fields      = result->Fetch();
            uint32 loguid      = fields[0].GetUInt32();
            uint32 respawnTime = fields[1].GetUInt32();

            _goRespawnTimes[loguid] = time_t(respawnTime);
        }
        while (result->NextRow());
    }
}

void Map::DeleteRespawnTimes()
{
    _creatureRespawnTimes.clear();
    _goRespawnTimes.clear();

    DeleteRespawnTimesInDB(GetId(), GetInstanceId());
}

void Map::DeleteRespawnTimesInDB(uint16 mapId, uint32 instanceId)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CREATURE_RESPAWN_BY_INSTANCE);
    stmt->setUInt16(0, mapId);
    stmt->setUInt32(1, instanceId);
    CharacterDatabase.Execute(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GO_RESPAWN_BY_INSTANCE);
    stmt->setUInt16(0, mapId);
    stmt->setUInt32(1, instanceId);
    CharacterDatabase.Execute(stmt);
}

time_t Map::GetLinkedRespawnTime(uint64 guid) const
{
    uint64 linkedGuid = sObjectMgr->GetLinkedRespawnGuid(guid);
    switch (GUID_HIPART(linkedGuid))
    {
        case HIGHGUID_UNIT:
            return GetCreatureRespawnTime(GUID_LOPART(linkedGuid));
        case HIGHGUID_GAMEOBJECT:
            return GetGORespawnTime(GUID_LOPART(linkedGuid));
        default:
            break;
    }

    return time_t(0);
}

void Map::loadGridsInRange(Position const &center, float radius)
{
    auto const x = center.GetPositionX();
    auto const y = center.GetPositionY();

    CellCoord standingCellCoord(Trinity::ComputeCellCoord(x, y));
    if (!standingCellCoord.IsCoordValid())
        return;

    if (radius > SIZE_OF_GRIDS)
        radius = SIZE_OF_GRIDS;

    CellArea area = Cell::CalculateCellArea(x, y, radius);
    if (!area)
        return;

    for (uint32 x = area.low_bound.x_coord; x <= area.high_bound.x_coord; ++x)
    {
        for (uint32 y = area.low_bound.y_coord; y <= area.high_bound.y_coord; ++y)
        {
            CellCoord cellCoord(x, y);
            if (cellCoord != standingCellCoord)
                EnsureGridLoaded(Cell(cellCoord));
        }
    }
}
