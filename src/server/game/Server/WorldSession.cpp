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

/** \file
    \ingroup u2w
*/

#include "WorldSocket.h"                                    // must be first to make ACE happy with ACE includes in it
#include "Common.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "Vehicle.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "Group.h"
#include "Guild.h"
#include "World.h"
#include "ObjectAccessor.h"
#include "BattlegroundMgr.h"
#include "OutdoorPvPMgr.h"
#include "MapManager.h"
#include "SocialMgr.h"
#include "zlib.h"
#include "ScriptMgr.h"
#include "Transport.h"
#include "WardenWin.h"
#include "AccountMgr.h"

#include <zlib.h>

bool MapSessionFilter::Process(WorldPacket const *packet) const
{
    OpcodeHandler const * const handler = opcodeTable[WOW_CLIENT][packet->GetOpcode()];

    // let's check if our opcode can be really processed in Map::Update()
    if (handler->packetProcessing == PROCESS_INPLACE)
        return true;

    // we do not process thread-unsafe packets
    if (handler->packetProcessing == PROCESS_THREADUNSAFE)
        return false;

    // in Map::Update() we do not process packets where player is not in world!
    Player const * const player = m_session->GetPlayer();
    return player && player->IsInWorld();
}

bool WorldSessionFilter::Process(WorldPacket const *packet) const
{
    OpcodeHandler const * const handler = opcodeTable[WOW_CLIENT][packet->GetOpcode()];

    // check if packet handler is supposed to be safe
    if (handler->packetProcessing == PROCESS_INPLACE)
        return true;

    // thread-unsafe packets should be processed in World::UpdateSessions()
    if (handler->packetProcessing == PROCESS_THREADUNSAFE)
        return true;

    // lets process all packets for non-in-the-world player
    Player const * const player = m_session->GetPlayer();
    return !(player && player->IsInWorld());
}

/// WorldSession constructor
WorldSession::WorldSession(uint32 id, WorldSocket* sock, AccountTypes sec,
                           uint8 expansion, time_t mute_time,
                           LocaleConstant locale, uint32 recruiter,
                           bool isARecruiter)
    : m_muteTime(mute_time), m_timeOutTime(0), _player(NULL), m_Socket(sock)
    , _security(sec), _accountId(id), m_expansion(expansion), _logoutTime(0)
    , m_inQueue(false), m_playerLoading(false), m_playerLogout(false)
    , m_playerRecentlyLogout(false), m_playerSave(false)
    , m_sessionDbcLocale(sWorld->GetAvailableDbcLocale(locale))
    , m_sessionDbLocaleIndex(locale), m_latency(0), m_TutorialsChanged(false)
    , recruiterId(recruiter), isRecruiter(isARecruiter), timeLastWhoCommand(0)
    , timeCharEnumOpcode(0), timeLastChannelInviteCommand(0)
    , timeLastChannelPassCommand(0), timeLastChannelMuteCommand(0)
    , timeLastChannelBanCommand(0), timeLastChannelUnbanCommand(0)
    , timeLastChannelAnnounceCommand(0), timeLastGroupInviteCommand(0)
    , timeLastGuildInviteCommand(0), timeLastChannelModerCommand(0)
    , timeLastChannelOwnerCommand(0), timeLastChannelSetownerCommand(0)
    , timeLastChannelUnmoderCommand(0), timeLastChannelUnmuteCommand(0)
    , timeLastChannelKickCommand(0), timeLastServerCommand(0)
    , timeLastArenaTeamCommand(0), timeLastCalendarInvCommand(0)
    , timeLastChangeSubGroupCommand(0), timeLastSellItemOpcode(0)
{
        _warden = NULL;
    _filterAddonMessages = false;

    if (sock)
    {
        m_Address = sock->GetRemoteAddress();
        sock->AddReference();
        ResetTimeOutTime();
        LoginDatabase.PExecute("UPDATE account SET online = 1 WHERE id = %u;", GetAccountId());     // One-time query
    }

    InitializeQueryCallbackParameters();
}

/// WorldSession destructor
WorldSession::~WorldSession()
{
    ///- unload player if not unloaded
    if (_player)
        LogoutPlayer (true);

    /// - If have unclosed socket, close it
    if (m_Socket)
    {
        m_Socket->CloseSocket();
        m_Socket->RemoveReference();
        m_Socket = NULL;
    }

    if (_warden)
        delete _warden;

    ///- empty incoming packet queue
    WorldPacket* packet = NULL;
    while (_recvQueue.next(packet))
        delete packet;

    LoginDatabase.PExecute("UPDATE account SET online = 0 WHERE id = %u;", GetAccountId());     // One-time query
}

/// Get the player name
std::string WorldSession::GetPlayerName(bool simple /* = true */) const
 {
    std::string name = "[Player: ";
    uint32 guidLow = 0;

    if (Player* player = GetPlayer())
    {
        name.append(player->GetName());
        guidLow = player->GetGUIDLow();
    }
    else
        name.append("<none>");

    if (!simple)
    {
        std::ostringstream ss;
        ss << " (Guid: " << guidLow << ", Account: " << GetAccountId() << ")";
        name.append(ss.str());
    }

    name.append("]");
    return name;
}

/// Get player guid if available. Use for logging purposes only
uint32 WorldSession::GetGuidLow() const
{
    return GetPlayer() ? GetPlayer()->GetGUIDLow() : 0;
}

/// Send a packet to the client
void WorldSession::SendPacket(WorldPacket const* packet, bool forced /*= false*/)
{
    //ASSERT(packet->GetBitPos() == 8 && "WorldPacket::FlushBits was not called.");

    if (!m_Socket)
        return;

    if (packet->GetOpcode() == NULL_OPCODE && !forced)
    {
        TC_LOG_DEBUG("network.opcode", "Prevented sending of NULL_OPCODE to %s", GetPlayerName(false).c_str());
        return;
    }
    else if (packet->GetOpcode() == UNKNOWN_OPCODE && !forced)
    {
        TC_LOG_ERROR("network.opcode", "Prevented sending of UNKNOWN_OPCODE to %s", GetPlayerName(false).c_str());
        return;
    }

    if (!forced)
    {
        OpcodeHandler* handler = opcodeTable[WOW_SERVER][packet->GetOpcode()];
        if (!handler || handler->status == STATUS_UNHANDLED)
        {
            TC_LOG_ERROR("network.opcode", "Prevented sending disabled opcode 0x%04X to player %s, account %u",
                         packet->GetOpcode(), GetPlayerName().c_str(), GetAccountId());
            return;
        }
    }

#ifdef TRINITY_DEBUG
    // Code for network use statistic
    static uint64 sendPacketCount = 0;
    static uint64 sendPacketBytes = 0;

    static time_t firstTime = time(NULL);
    static time_t lastTime = firstTime;                     // next 60 secs start time

    static uint64 sendLastPacketCount = 0;
    static uint64 sendLastPacketBytes = 0;

    time_t cur_time = time(NULL);

    if ((cur_time - lastTime) < 60)
    {
        sendPacketCount+=1;
        sendPacketBytes+=packet->size();

        sendLastPacketCount+=1;
        sendLastPacketBytes+=packet->size();
    }
    else
    {
        uint64 minTime = uint64(cur_time - lastTime);
        uint64 fullTime = uint64(lastTime - firstTime);
        TC_LOG_INFO("misc", "Send all time packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f time: %u", sendPacketCount, sendPacketBytes, float(sendPacketCount)/fullTime, float(sendPacketBytes)/fullTime, uint32(fullTime));
        TC_LOG_INFO("misc", "Send last min packets count: " UI64FMTD " bytes: " UI64FMTD " avr.count/sec: %f avr.bytes/sec: %f", sendLastPacketCount, sendLastPacketBytes, float(sendLastPacketCount)/minTime, float(sendLastPacketBytes)/minTime);

        lastTime = cur_time;
        sendLastPacketCount = 1;
        sendLastPacketBytes = packet->wpos();               // wpos is real written size
    }
#endif                                                      // !TRINITY_DEBUG

    if (m_Socket->SendPacket(packet) == -1)
        m_Socket->CloseSocket();
}

/// Add an incoming packet to the queue
void WorldSession::QueuePacket(WorldPacket* new_packet)
{
    _recvQueue.add(new_packet);
}

/// Logging helper for unexpected opcodes
void WorldSession::LogUnexpectedOpcode(WorldPacket* packet, const char* status, const char *reason)
{
    TC_LOG_ERROR("network.opcode", "Received unexpected opcode 0x%04X Status: %s Reason: %s from player %s, account %u",
                 packet->GetOpcode(), status, reason, GetPlayerName().c_str(), GetAccountId());
}

/// Logging helper for unexpected opcodes
void WorldSession::LogUnprocessedTail(WorldPacket* packet)
{
    if (!sLog->ShouldLog("network.opcode", LOG_LEVEL_TRACE) || packet->rpos() >= packet->wpos())
        return;

    TC_LOG_TRACE("network.opcode", "Unprocessed tail data (read stop at " SIZEFMTD " from " SIZEFMTD ") Opcode 0x%04X from player %s, account %u",
                 packet->rpos(), packet->wpos(), packet->GetOpcode(), GetPlayerName().c_str(), GetAccountId());
    TC_LOG_TRACE("network.opcode", "%s", packet->hexlike().c_str());
}

struct OpcodeInfo
{
    OpcodeInfo(uint32 nb, uint32 time) : nbPkt(nb), totalTime(time) {}
    uint32 nbPkt;
    uint32 totalTime;
};

/// Update the WorldSession (triggered by World update)
bool WorldSession::Update(uint32 diff, PacketFilter const &filter)
{
    /// Update Timeout timer.
    UpdateTimeOutTime(diff);

    ///- Before we process anything:
    /// If necessary, kick the player from the character select screen
    if (IsConnectionIdle())
        m_Socket->CloseSocket();

    ///- Retrieve packets from the receive queue and call the appropriate handlers
    /// not process packets if socket already closed
    WorldPacket* packet = NULL;
    //! Delete packet after processing by default
    bool deletePacket = true;
    //! To prevent infinite loop
    WorldPacket* firstDelayedPacket = NULL;
    //! If _recvQueue.peek() == firstDelayedPacket it means that in this Update call, we've processed all
    //! *properly timed* packets, and we're now at the part of the queue where we find
    //! delayed packets that were re-enqueued due to improper timing. To prevent an infinite
    //! loop caused by re-enqueueing the same packets over and over again, we stop updating this session
    //! and continue updating others. The re-enqueued packets will be handled in the next Update call for this session.
    uint32 processedPackets = 0;
    while (m_Socket && !m_Socket->IsClosed() &&
            !_recvQueue.empty() && _recvQueue.peek(true) != firstDelayedPacket &&
            _recvQueue.next(packet, filter))
    {
        const OpcodeHandler* opHandle = opcodeTable[WOW_CLIENT][packet->GetOpcode()];

        try
        {
            switch (opHandle->status)
            {
                case STATUS_LOGGEDIN:
                    if (!_player)
                    {
                        // skip STATUS_LOGGEDIN opcode unexpected errors if player logout sometime ago - this can be network lag delayed packets
                        //! If player didn't log out a while ago, it means packets are being sent while the server does not recognize
                        //! the client to be in world yet. We will re-add the packets to the bottom of the queue and process them later.
                        if (!m_playerRecentlyLogout)
                        {
                            //! Prevent infinite loop
                            if (!firstDelayedPacket)
                                firstDelayedPacket = packet;
                            //! Because checking a bool is faster than reallocating memory
                            deletePacket = false;
                            QueuePacket(packet);
                            //! Log
                            TC_LOG_DEBUG("network", "Re-enqueueing packet with opcode 0x%04X with with status STATUS_LOGGEDIN. "
                                         "Player is currently not in world yet.", packet->GetOpcode());
                        }
                    }
                    else if (_player->IsInWorld())
                    {
                        (this->*opHandle->handler)(*packet);
                        if (sLog->ShouldLog("network", LOG_LEVEL_TRACE) && packet->rpos() < packet->wpos())
                            LogUnprocessedTail(packet);
                    }
                    // lag can cause STATUS_LOGGEDIN opcodes to arrive after the player started a transfer
                    break;
                case STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT:
                    if (!_player && !m_playerRecentlyLogout && !m_playerLogout) // There's a short delay between _player = null and m_playerRecentlyLogout = true during logout
                        LogUnexpectedOpcode(packet, "STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT",
                            "the player has not logged in yet and not recently logout");
                    else
                    {
                        // not expected _player or must checked in packet hanlder
                        (this->*opHandle->handler)(*packet);
                        if (sLog->ShouldLog("network", LOG_LEVEL_TRACE) && packet->rpos() < packet->wpos())
                            LogUnprocessedTail(packet);
                    }
                    break;
                case STATUS_TRANSFER:
                    if (!_player)
                        LogUnexpectedOpcode(packet, "STATUS_TRANSFER", "the player has not logged in yet");
                    else if (_player->IsInWorld())
                        LogUnexpectedOpcode(packet, "STATUS_TRANSFER", "the player is still in world");
                    else
                    {
                        (this->*opHandle->handler)(*packet);
                        if (sLog->ShouldLog("network", LOG_LEVEL_TRACE) && packet->rpos() < packet->wpos())
                            LogUnprocessedTail(packet);
                    }
                    break;
                case STATUS_AUTHED:
                    // prevent cheating with skip queue wait
                    if (m_inQueue)
                    {
                        LogUnexpectedOpcode(packet, "STATUS_AUTHED", "the player not pass queue yet");
                        break;
                    }

                    // some auth opcodes can be recieved before STATUS_LOGGEDIN_OR_RECENTLY_LOGGOUT opcodes
                    // however when we recieve CMSG_CHAR_ENUM we are surely no longer during the logout process.
                    if (packet->GetOpcode() == CMSG_CHAR_ENUM)
                        m_playerRecentlyLogout = false;

                    (this->*opHandle->handler)(*packet);
                    if (sLog->ShouldLog("network", LOG_LEVEL_TRACE) && packet->rpos() < packet->wpos())
                        LogUnprocessedTail(packet);
                    break;
                case STATUS_NEVER:
                        TC_LOG_ERROR("network.opcode", "Received not allowed opcode %u from %s", packet->GetOpcode(), GetPlayerName(false).c_str());
                    break;
                case STATUS_UNHANDLED:
                        TC_LOG_ERROR("network.opcode", "Received not handled opcode %u from %s", packet->GetOpcode(), GetPlayerName(false).c_str());
                    break;
            }
        }
        catch(ByteBufferException &)
        {
            TC_LOG_ERROR("network", "WorldSession::Update ByteBufferException occured while parsing a packet (opcode: %u) from client %s, accountid=%i. Skipped packet.",
                    packet->GetOpcode(), GetRemoteAddress().c_str(), GetAccountId());
            TC_LOG_TRACE("network", "%s", packet->hexlike().c_str());
        }

        if (deletePacket)
            delete packet;

#define MAX_PROCESSED_PACKETS_IN_SAME_WORLDSESSION_UPDATE 250
        processedPackets++;

        //process only a max amout of packets in 1 Update() call.
        //Any leftover will be processed in next update
        if (processedPackets > MAX_PROCESSED_PACKETS_IN_SAME_WORLDSESSION_UPDATE)
            break;
    }

    ProcessQueryCallbacks();

    // code below is safe to execute in World::UpdateSessions only
    if (!filter.ProcessLogout())
        return true;

    // All remove packets are parsed and data is queued to produce exactly one
    // transaction. Should avoid deadlocks that cause duplicate items.
    processAuctionsToRemove();

    if (m_Socket && !m_Socket->IsClosed() && _warden)
        _warden->Update();

    time_t currTime = std::time(nullptr);
    if (ShouldLogOut(currTime) && !m_playerLoading)
        LogoutPlayer(true);

    if (m_Socket && m_Socket->IsClosed())
    {
        m_Socket->RemoveReference();
        m_Socket = nullptr;
    }

    return m_Socket != nullptr;
}

/// %Log the player out
void WorldSession::LogoutPlayer(bool Save)
{
    // finish pending transfers before starting the logout
    while (_player && _player->IsBeingTeleportedFar())
        HandleMoveWorldportAckOpcode();

    m_playerLogout = true;
    m_playerSave = Save;

    if (_player)
    {
        if (uint64 lguid = _player->GetLootGUID())
            DoLootRelease(lguid);

        ///- If the player just died before logging out, make him appear as a ghost
        //FIXME: logout must be delayed in case lost connection with client in time of combat
        if (_player->GetDeathTimer())
        {
            _player->getHostileRefManager().deleteReferences();
            _player->BuildPlayerRepop();
            _player->RepopAtGraveyard();
        }
        else if (!_player->getAttackers().empty())
        {
            // build set of player who attack _player or who have pet attacking of _player
            std::set<Player*> aset;
            for (Unit::AttackerSet::const_iterator itr = _player->getAttackers().begin(); itr != _player->getAttackers().end(); ++itr)
            {
                Unit* owner = (*itr)->GetOwner();           // including player controlled case
                if (owner && owner->GetTypeId() == TYPEID_PLAYER)
                    aset.insert(owner->ToPlayer());
                else if ((*itr)->GetTypeId() == TYPEID_PLAYER)
                    aset.insert((Player*)(*itr));
            }
            // CombatStop() method is removing all attackers from the AttackerSet
            // That is why it must be AFTER building current set of attackers
            _player->CombatStop();
            _player->getHostileRefManager().setOnlineOfflineState(false);
            _player->RemoveAllAurasOnDeath();
            _player->SetPvPDeath(!aset.empty());
            _player->KillPlayer();
            _player->BuildPlayerRepop();
            _player->RepopAtGraveyard();

            // give honor to all attackers from set like group case
            for (std::set<Player*>::const_iterator itr = aset.begin(); itr != aset.end(); ++itr)
                (*itr)->RewardHonor(_player, aset.size());

            // give bg rewards and update counters like kill by first from attackers
            // this can't be called for all attackers.
            if (!aset.empty())
                if (Battleground* bg = _player->GetBattleground())
                    bg->HandleKillPlayer(_player, *aset.begin());
        }
        else if (_player->HasAuraType(SPELL_AURA_SPIRIT_OF_REDEMPTION))
        {
            // this will kill character by SPELL_AURA_SPIRIT_OF_REDEMPTION
            _player->RemoveAurasByType(SPELL_AURA_MOD_SHAPESHIFT);
            _player->KillPlayer();
            _player->BuildPlayerRepop();
            _player->RepopAtGraveyard();
        }
        else if (_player->HasPendingBind())
        {
            _player->RepopAtGraveyard();
            _player->SetPendingBind(0, 0);
        }

        //drop a flag if player is carrying it
        if (Battleground* bg = _player->GetBattleground())
            bg->EventPlayerLoggedOut(_player);

        ///- Teleport to home if the player is in an invalid instance
        if (!_player->m_InstanceValid && !_player->isGameMaster())
            _player->TeleportTo(_player->m_homebindMapId, _player->m_homebindX, _player->m_homebindY, _player->m_homebindZ, _player->GetOrientation());

        sOutdoorPvPMgr->HandlePlayerLeaveZone(_player, _player->GetZoneId());

        for (int i=0; i < PLAYER_MAX_BATTLEGROUND_QUEUES; ++i)
        {
            if (BattlegroundQueueTypeId bgQueueTypeId = _player->GetBattlegroundQueueTypeId(i))
            {
                _player->RemoveBattlegroundQueueId(bgQueueTypeId);
                sBattlegroundMgr->GetBattlegroundQueue(bgQueueTypeId).RemovePlayer(_player->GetGUID(), true);
            }
        }

        // Repop at GraveYard or other player far teleport will prevent saving player because of not present map
        // Teleport player immediately for correct player save
        while (_player->IsBeingTeleportedFar())
            HandleMoveWorldportAckOpcode();

        ///- If the player is in a guild, update the guild roster and broadcast a logout message to other guild members
        if (Guild* guild = sGuildMgr->GetGuildById(_player->GetGuildId()))
            guild->HandleMemberLogout(this);

        ///- Remove pet
        _player->RemovePet(PET_REMOVE_DISMISS, PET_REMOVE_FLAG_RETURN_REAGENT);

        ///- empty buyback items and save the player in the database
        // some save parts only correctly work in case player present in map/player_lists (pets, etc)
        if (Save)
        {
            uint32 eslot;
            for (int j = BUYBACK_SLOT_START; j < BUYBACK_SLOT_END; ++j)
            {
                eslot = j - BUYBACK_SLOT_START;
                _player->SetUInt64Value(PLAYER_FIELD_VENDORBUYBACK_SLOT_1 + (eslot * 2), 0);
                _player->SetUInt32Value(PLAYER_FIELD_BUYBACK_PRICE_1 + eslot, 0);
                _player->SetUInt32Value(PLAYER_FIELD_BUYBACK_TIMESTAMP_1 + eslot, 0);
            }
            _player->SaveToDB();
        }

        ///- Leave all channels before player delete...
        _player->CleanupChannels();

        ///- If the player is in a group (or invited), remove him. If the group if then only 1 person, disband the group.
        _player->UninviteFromGroup();

        // remove player from the group if he is:
        // a) in group; b) not in raid group; c) logging out normally (not being kicked or disconnected)
        if (_player->GetGroup() && !_player->GetGroup()->isRaidGroup() && m_Socket)
            _player->RemoveFromGroup();

        //! Send update to group and reset stored max enchanting level
        if (_player->GetGroup())
        {
            _player->GetGroup()->SendUpdate();
            _player->GetGroup()->ResetMaxEnchantingLevel();
        }

        //! Broadcast a logout message to the player's friends
        sSocialMgr->SendFriendStatus(_player, FRIEND_OFFLINE, _player->GetGUID(), true);
        sSocialMgr->RemovePlayerSocial(_player->GetGUIDLow());

        // forfiet any pet battles in progress
        sPetBattleSystem->ForfietBattle(_player->GetGUID());

        //! Call script hook before deletion
        sScriptMgr->OnPlayerLogout(_player);

        //! Remove the player from the world
        // the player may not be in the world when logging out
        // e.g if he got disconnected during a transfer to another map
        // calls to GetMap in this case may cause crashes
        _player->CleanupsBeforeDelete();
        TC_LOG_INFO("entities.player.character", "Account: %d (IP: %s) Logout Character:[%s] (GUID: %u) Level: %d",
                    GetAccountId(), GetRemoteAddress().c_str(), _player->GetName().c_str(), _player->GetGUIDLow(), _player->getLevel());
        if (Map* _map = _player->FindMap())
            _map->RemovePlayerFromMap(_player, true);

        SetPlayer(NULL); //! Pointer already deleted during RemovePlayerFromMap

        //! Send the 'logout complete' packet to the client
        //! Client will respond by sending 3x CMSG_CANCEL_TRADE, which we currently dont handle
        WorldPacket data(SMSG_LOGOUT_COMPLETE, 0);
        SendPacket(&data);
        TC_LOG_DEBUG("network", "SESSION: Sent SMSG_LOGOUT_COMPLETE Message");

        //! Since each account can only have one online character at any given time, ensure all characters for active account are marked as offline
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ACCOUNT_ONLINE);
        stmt->setUInt32(0, GetAccountId());
        CharacterDatabase.Execute(stmt);
    }

    m_playerLogout = false;
    m_playerSave = false;
    m_playerRecentlyLogout = true;
    LogoutRequest(0);
}

/// Kick a player out of the World
void WorldSession::KickPlayer()
{
    if (m_Socket)
        m_Socket->CloseSocket();
}

void WorldSession::SendNotification(const char *format, ...)
{
    if (format)
    {
        va_list ap;
        char szStr[1024];
        szStr[0] = '\0';
        va_start(ap, format);
        vsnprintf(szStr, 1024, format, ap);
        va_end(ap);

        size_t len = strlen(szStr);
        WorldPacket data(SMSG_NOTIFICATION, 2 + len);
        data.WriteBits(len, 12);
        data.FlushBits();
        data.append(szStr, len);
        SendPacket(&data);
    }
}

void WorldSession::SendNotification(uint32 string_id, ...)
{
    char const* format = GetTrinityString(string_id);
    if (format)
    {
        va_list ap;
        char szStr[1024];
        szStr[0] = '\0';
        va_start(ap, string_id);
        vsnprintf(szStr, 1024, format, ap);
        va_end(ap);

        size_t len = strlen(szStr);
        WorldPacket data(SMSG_NOTIFICATION, 2 + len);
        data.WriteBits(len, 12);
        data.FlushBits();
        data.append(szStr, len);
        SendPacket(&data);
    }
}

const char *WorldSession::GetTrinityString(int32 entry) const
{
    return sObjectMgr->GetTrinityString(entry, GetSessionDbLocaleIndex());
}

void WorldSession::Handle_NULL(WorldPacket& recvPacket)
{
    TC_LOG_ERROR("network.opcode", "Received unhandled opcode 0x%04X from %s",
                 recvPacket.GetOpcode(), GetPlayerName(false).c_str());
}

void WorldSession::Handle_EarlyProccess(WorldPacket& recvPacket)
{
    TC_LOG_ERROR("network.opcode", "Received opcode 0x%04X that must be processed in WorldSocket::OnRead from %s",
                 recvPacket.GetOpcode(), GetPlayerName(false).c_str());
}

void WorldSession::Handle_ServerSide(WorldPacket& recvPacket)
{
    TC_LOG_ERROR("network.opcode", "Received server-side opcode 0x%04X from %s",
                 recvPacket.GetOpcode(), GetPlayerName(false).c_str());
}

void WorldSession::Handle_Deprecated(WorldPacket& recvPacket)
{
    TC_LOG_ERROR("network.opcode", "Received deprecated opcode 0x%04X from %s",
                 recvPacket.GetOpcode(), GetPlayerName(false).c_str());
}

void WorldSession::SendAuthWaitQue(uint32 position)
{
    if (position == 0)
        SendAuthResponse(AUTH_OK, false);
    else
        SendAuthResponse(AUTH_OK, true, position);
}

void WorldSession::LoadGlobalAccountData()
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_ACCOUNT_DATA);
    stmt->setUInt32(0, GetAccountId());
    LoadAccountData(CharacterDatabase.Query(stmt), GLOBAL_CACHE_MASK);
}

void WorldSession::LoadAccountData(PreparedQueryResult result, uint32 mask)
{
    for (uint32 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        if (mask & (1 << i))
            m_accountData[i] = AccountData();

    if (!result)
        return;

    do
    {
        Field* fields = result->Fetch();
        uint32 type = fields[0].GetUInt8();
        if (type >= NUM_ACCOUNT_DATA_TYPES)
        {
            TC_LOG_ERROR("misc", "Table `%s` have invalid account data type (%u), ignore.",
                mask == GLOBAL_CACHE_MASK ? "account_data" : "character_account_data", type);
            continue;
        }

        if ((mask & (1 << type)) == 0)
        {
            TC_LOG_ERROR("misc", "Table `%s` have non appropriate for table  account data type (%u), ignore.",
                mask == GLOBAL_CACHE_MASK ? "account_data" : "character_account_data", type);
            continue;
        }

        m_accountData[type].Time = time_t(fields[1].GetUInt32());
        m_accountData[type].Data = fields[2].GetString();
    }
    while (result->NextRow());
}

void WorldSession::SetAccountData(AccountDataType type, time_t tm, std::string data)
{
    uint32 id = 0;
    uint32 index = 0;
    if ((1 << type) & GLOBAL_CACHE_MASK)
    {
        id = GetAccountId();
        index = CHAR_REP_ACCOUNT_DATA;
    }
    else
    {
        // _player can be NULL and packet received after logout but m_GUID still store correct guid
        if (!m_GUIDLow)
            return;

        id = m_GUIDLow;
        index = CHAR_REP_PLAYER_ACCOUNT_DATA;
    }

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(index);
    stmt->setUInt32(0, id);
    stmt->setUInt8 (1, type);
    stmt->setUInt32(2, uint32(tm));
    stmt->setString(3, data);
    CharacterDatabase.Execute(stmt);

    m_accountData[type].Time = tm;
    m_accountData[type].Data = data;
}

void WorldSession::SendAccountDataTimes(uint32 mask)
{
    WorldPacket data(SMSG_ACCOUNT_DATA_TIMES, 4+NUM_ACCOUNT_DATA_TYPES*4+4+1);

    data << uint32(time(NULL));                             // Server time

    for (uint32 i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        data << uint32(GetAccountData(AccountDataType(i))->Time);// also unix time

    data << uint32(mask);                                   // type mask
    data.WriteBit(0);

    data.FlushBits();

    SendPacket(&data);
}

void WorldSession::LoadTutorialsData()
{
    memset(m_Tutorials, 0, sizeof(uint32) * MAX_ACCOUNT_TUTORIAL_VALUES);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_TUTORIALS);
    stmt->setUInt32(0, GetAccountId());
    if (PreparedQueryResult result = CharacterDatabase.Query(stmt))
        for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
            m_Tutorials[i] = (*result)[i].GetUInt32();

    m_TutorialsChanged = false;
}

void WorldSession::SendTutorialsData()
{
    WorldPacket data(SMSG_TUTORIAL_FLAGS, 4 * MAX_ACCOUNT_TUTORIAL_VALUES);
    for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        data << m_Tutorials[i];
    SendPacket(&data);
}

void WorldSession::SaveTutorialsData(SQLTransaction &trans)
{
    if (!m_TutorialsChanged)
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_HAS_TUTORIALS);
    stmt->setUInt32(0, GetAccountId());
    bool hasTutorials = CharacterDatabase.Query(stmt) != NULL;
    // Modify data in DB
    stmt = CharacterDatabase.GetPreparedStatement(hasTutorials ? CHAR_UPD_TUTORIALS : CHAR_INS_TUTORIALS);
    for (uint8 i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        stmt->setUInt32(i, m_Tutorials[i]);
    stmt->setUInt32(MAX_ACCOUNT_TUTORIAL_VALUES, GetAccountId());
    trans->Append(stmt);

    m_TutorialsChanged = false;
}

void WorldSession::ReadAddonsInfo(WorldPacket &data)
{
    if (data.rpos() + 4 > data.size())
        return;

    uint32 size;
    data >> size;

    if (!size)
        return;

    if (size > 0xFFFFF)
    {
        TC_LOG_ERROR("misc", "WorldSession::ReadAddonsInfo addon info too big, size %u", size);
        return;
    }

    uLongf uSize = size;

    uint32 pos = data.rpos();

    ByteBuffer addonInfo;
    addonInfo.resize(size);

    if (uncompress(addonInfo.contents(), &uSize, data.contents() + pos, data.size() - pos) == Z_OK)
    {
        uint32 addonsCount;
        addonInfo >> addonsCount;                         // addons count

        for (uint32 i = 0; i < addonsCount; ++i)
        {
            std::string addonName;
            uint8 enabled;
            uint32 crc, unk1;

            // check next addon data format correctness
            if (addonInfo.rpos() + 1 > addonInfo.size())
                return;

            addonInfo >> addonName;

            addonInfo >> enabled >> crc >> unk1;

            TC_LOG_INFO("misc", "ADDON: Name: %s, Enabled: 0x%x, CRC: 0x%x, Unknown2: 0x%x", addonName.c_str(), enabled, crc, unk1);

            AddonInfo addon(addonName, enabled, crc, 2, true);

            SavedAddon const* savedAddon = AddonMgr::GetAddonInfo(addonName);
            if (savedAddon)
            {
                bool match = true;

                if (addon.CRC != savedAddon->CRC)
                    match = false;

                if (!match)
                    TC_LOG_INFO("misc", "ADDON: %s was known, but didn't match known CRC (0x%x)!", addon.Name.c_str(), savedAddon->CRC);
                else
                    TC_LOG_INFO("misc", "ADDON: %s was known, CRC is correct (0x%x)", addon.Name.c_str(), savedAddon->CRC);
            }
            else
            {
                AddonMgr::SaveAddon(addon);

                TC_LOG_INFO("misc", "ADDON: %s (0x%x) was not known, saving...", addon.Name.c_str(), addon.CRC);
            }

            // TODO: Find out when to not use CRC/pubkey, and other possible states.
            m_addonsList.push_back(addon);
        }

        uint32 currentTime;
        addonInfo >> currentTime;
        TC_LOG_DEBUG("network", "ADDON: CurrentTime: %u", currentTime);

        if (addonInfo.rpos() != addonInfo.size())
            TC_LOG_DEBUG("network", "packet under-read!");
    }
    else
        TC_LOG_ERROR("misc", "Addon packet uncompress error!");
}

void WorldSession::SendAddonsInfo()
{
    uint8 addonPublicKey[256] =
    {
        0xD4, 0xED, 0xF2, 0x84, 0x34, 0x94, 0xC6, 0xA3, 0xC3, 0x56, 0x59, 0xB4, 0x45, 0x0E, 0xD5, 0xF0,
        0xFB, 0x08, 0x22, 0x0E, 0x50, 0x43, 0x01, 0x64, 0x9B, 0x03, 0x68, 0x7E, 0xB7, 0x5A, 0x44, 0x8C,
        0xBE, 0x40, 0x45, 0xEF, 0x56, 0x04, 0xFF, 0x4A, 0x8B, 0x10, 0x51, 0x05, 0xA6, 0xCB, 0x2B, 0x0F,
        0x86, 0xBE, 0xB5, 0xDA, 0x57, 0x29, 0x10, 0x64, 0xDD, 0xEE, 0x3C, 0xFB, 0x72, 0xBA, 0x37, 0x72,
        0x98, 0xE0, 0x22, 0x40, 0x8E, 0x8F, 0x54, 0x1E, 0x7A, 0xB9, 0x0E, 0xCE, 0x34, 0xD0, 0xA8, 0xAD,
        0x22, 0xFE, 0xD7, 0x20, 0x11, 0x50, 0xFE, 0x3D, 0xCF, 0x31, 0x36, 0x49, 0x79, 0x1B, 0x1C, 0xBC,
        0xB9, 0x50, 0x1A, 0x9E, 0xEA, 0x5A, 0x79, 0x23, 0xD9, 0x6D, 0xCC, 0x13, 0x24, 0x5D, 0xED, 0x6F,
        0xC3, 0x8C, 0x44, 0x2A, 0x89, 0x85, 0x36, 0x48, 0x50, 0xBB, 0x81, 0xA3, 0x9E, 0x88, 0x2D, 0x5A,
        0xFB, 0x1E, 0x97, 0xD6, 0xD7, 0xB6, 0x84, 0x45, 0xC3, 0x92, 0xF7, 0x6E, 0x77, 0xCA, 0x10, 0x2F,
        0xBB, 0x75, 0x16, 0xF5, 0x69, 0xF3, 0xC2, 0x63, 0xD1, 0x8F, 0x0F, 0x42, 0xCE, 0x8F, 0x5B, 0x4B,
        0xCF, 0xF0, 0x77, 0x6B, 0xFC, 0x69, 0xF9, 0x15, 0x0D, 0x7A, 0x79, 0x14, 0x91, 0x7C, 0x57, 0x54,
        0x9A, 0x4B, 0x0D, 0xE1, 0xB3, 0x0F, 0xC8, 0x5D, 0x3D, 0x18, 0xA8, 0xE0, 0x30, 0x0E, 0x0A, 0x1B,
        0x8F, 0xCB, 0x85, 0xB9, 0x44, 0xC3, 0x85, 0xAD, 0xE5, 0x87, 0xC7, 0x2C, 0xCF, 0x8B, 0xC7, 0xDB,
        0x5E, 0x8B, 0xD2, 0x68, 0x31, 0xF0, 0xFC, 0x2D, 0xE1, 0x17, 0xBA, 0x01, 0x80, 0x46, 0x8A, 0xAA,
        0x02, 0x0A, 0x76, 0x34, 0xFA, 0x28, 0x3C, 0x9C, 0x95, 0x93, 0x64, 0x3F, 0xD8, 0xFB, 0x16, 0xEF,
        0xA2, 0x2C, 0xC6, 0x6E, 0x8B, 0x32, 0x07, 0x6F, 0x3E, 0xE7, 0x5D, 0x0D, 0x4E, 0xDF, 0x85, 0x8C
    };

    WorldPacket data(SMSG_ADDON_INFO, 4);

    data.WriteBits(m_addonsList.size(), 23);

    for (AddonsList::iterator itr = m_addonsList.begin(); itr != m_addonsList.end(); ++itr)
    {
        bool hasString = 0;
        data.WriteBit(hasString);

        bool usePublicKey = (itr->CRC != STANDARD_ADDON_CRC);
        data.WriteBit(usePublicKey); // Addon public key

        if (hasString)
            data.WriteBits(0, 8); // String size

        bool crcpub = itr->UsePublicKeyOrCRC;
        data.WriteBit(crcpub);
    }

    data.WriteBits(0, 18); // count for an unknown for loop

    for (AddonsList::iterator itr = m_addonsList.begin(); itr != m_addonsList.end(); ++itr)
    {
        bool crcpub = itr->UsePublicKeyOrCRC;

        bool unk3 = 0;
        if (unk3)
            data.WriteString("");

        bool usePublicKey = (itr->CRC != STANDARD_ADDON_CRC);
        if (usePublicKey)
        {
            TC_LOG_INFO("misc", "ADDON: CRC (0x%x) for addon %s is wrong (does not match expected 0x%x), sending pubkey",
                itr->CRC, itr->Name.c_str(), STANDARD_ADDON_CRC);

            data.append(addonPublicKey, sizeof(addonPublicKey));
        }

        if (crcpub)
        {
            data << uint8(1); // unk 2
            data << uint32(0);// unk 1
        }

        data << uint8(itr->State);
    }

    m_addonsList.clear();

    SendPacket(&data);
}

bool WorldSession::IsAddonRegistered(const std::string& prefix) const
{
    if (!_filterAddonMessages) // if we have hit the softcap (64) nothing should be filtered
        return true;

    if (_registeredAddonPrefixes.empty())
        return false;

    auto const itr = std::find(_registeredAddonPrefixes.begin(), _registeredAddonPrefixes.end(), prefix);
    return itr != _registeredAddonPrefixes.end();
}

void WorldSession::HandleUnregisterAddonPrefixesOpcode(WorldPacket& /*recvPacket*/) // empty packet
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_UNREGISTER_ALL_ADDON_PREFIXES");

    _registeredAddonPrefixes.clear();
}

void WorldSession::HandleAddonRegisteredPrefixesOpcode(WorldPacket& recvPacket)
{
    TC_LOG_DEBUG("network", "WORLD: Received CMSG_ADDON_REGISTERED_PREFIXES");

    // This is always sent after CMSG_UNREGISTER_ALL_ADDON_PREFIXES

    uint32 count = recvPacket.ReadBits(24);

    if (count > REGISTERED_ADDON_PREFIX_SOFTCAP)
    {
        // if we have hit the softcap (64) nothing should be filtered
        _filterAddonMessages = false;
        recvPacket.rfinish();
        return;
    }

    std::vector<uint8> lengths(count);
    for (uint32 i = 0; i < count; ++i)
        lengths[i] = recvPacket.ReadBits(5);

    for (uint32 i = 0; i < count; ++i)
        _registeredAddonPrefixes.push_back(recvPacket.ReadString(lengths[i]));

    if (_registeredAddonPrefixes.size() > REGISTERED_ADDON_PREFIX_SOFTCAP) // shouldn't happen
    {
        _filterAddonMessages = false;
        return;
    }

    _filterAddonMessages = true;
}

void WorldSession::SetPlayer(Player* player)
{
    _player = player;

    // set m_GUID that can be used while player loggined and later until m_playerRecentlyLogout not reset
    if (_player)
        m_GUIDLow = _player->GetGUIDLow();
}

void WorldSession::InitializeQueryCallbackParameters()
{
    // Callback parameters that have pointers in them should be properly
    // initialized to NULL here.
    _charCreateCallback.SetParam(NULL);
}

void WorldSession::ProcessQueryCallbacks()
{
    PreparedQueryResult result;

    //! HandleNameQueryOpcode
    {
        ACE_Guard<ACE_Thread_Mutex> guard(_nameQueryCallbacksLock);
        while (!_nameQueryCallbacks.empty())
        {
            NameQueryInfo &nameQuery = _nameQueryCallbacks.front();
            if (!nameQuery.second.ready())
                break;
            if (nameQuery.second.get(result) == 0)
                SendNameQueryOpcodeCallBack(nameQuery.first, result);
            _nameQueryCallbacks.pop_front();
        }
    }

    //! HandleCharEnumOpcode
    if (_charEnumCallback.ready())
    {
        _charEnumCallback.get(result);
        HandleCharEnum(result);
        _charEnumCallback.cancel();
    }

    if (_charCreateCallback.IsReady())
    {
        _charCreateCallback.GetResult(result);
        HandleCharCreateCallback(result, _charCreateCallback.GetParam());
        // Don't call FreeResult() here, the callback handler will do that depending on the events in the callback chain
    }

    //! HandlePlayerLoginOpcode
    if (_charLoginCallback.ready() && _authLoginCallback.ready())
    {
        SQLQueryHolder *charParam, *authParam;

        _charLoginCallback.get(charParam);
        _authLoginCallback.get(authParam);

        HandlePlayerLogin((CharLoginQueryHolder*)charParam, (AuthLoginQueryHolder*)authParam);

        _charLoginCallback.cancel();
        _authLoginCallback.cancel();
    }

    //! HandleAddFriendOpcode
    if (_addFriendCallback.IsReady())
    {
        std::string param = _addFriendCallback.GetParam();
        _addFriendCallback.GetResult(result);
        HandleAddFriendOpcodeCallBack(result, param);
        _addFriendCallback.FreeResult();
    }

    //- HandleCharRenameOpcode
    if (_charRenameCallback.IsReady())
    {
        std::string param = _charRenameCallback.GetParam();
        _charRenameCallback.GetResult(result);
        HandleChangePlayerNameOpcodeCallBack(result, param);
        _charRenameCallback.FreeResult();
    }

    //- HandleCharAddIgnoreOpcode
    if (_addIgnoreCallback.ready())
    {
        _addIgnoreCallback.get(result);
        HandleAddIgnoreOpcodeCallBack(result);
        _addIgnoreCallback.cancel();
    }

    //- SendStabledPet
    if (_sendPetListCallback.IsReady())
    {
        uint64 param = _sendPetListCallback.GetParam();
        _sendPetListCallback.GetResult(result);
        SendPetListCallback(result, param);
        _sendPetListCallback.FreeResult();
    }

    //- HandleSetPetSlotOpcode
    if (_setPetSlotCallback.IsReady())
    {
        _setPetSlotCallback.GetResult(result);
        SetPetSlotCallback(result, _setPetSlotCallback.GetParam());
        _setPetSlotCallback.FreeResult();
    }
}

void WorldSession::InitWarden(BigNumber* k, std::string const &os)
{
    if (os == "Win")
    {
        _warden = new WardenWin();
        _warden->Init(this, k);
    }
}
