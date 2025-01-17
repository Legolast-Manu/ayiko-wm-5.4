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

#include <ace/Message_Block.h>
#include <ace/OS_NS_string.h>
#include <ace/OS_NS_unistd.h>
#include <ace/os_include/arpa/os_inet.h>
#include <ace/os_include/netinet/os_tcp.h>
#include <ace/os_include/sys/os_types.h>
#include <ace/os_include/sys/os_socket.h>
#include <ace/OS_NS_string.h>
#include <ace/Reactor.h>
#include <ace/Auto_Ptr.h>

#include "WorldSocket.h"
#include "Common.h"

#include "Util.h"
#include "World.h"
#include "WorldPacket.h"
#include "SharedDefines.h"
#include "ByteBuffer.h"
#include "Opcodes.h"
#include "DatabaseEnv.h"
#include "BigNumber.h"
#include "SHA1.h"
#include "WorldSession.h"
#include "WorldSocketMgr.h"
#include "Log.h"
#include "PacketLog.h"
#include "ScriptMgr.h"
#include "AccountMgr.h"

namespace {

Opcodes DropHighBytes(Opcodes opcode)
{
    return Opcodes(opcode & 0xFFFF);
}

#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct ServerPktHeader
{
    enum { Length = 4 };

    /**
     * size is the length of the payload _plus_ the length of the opcode
     */
    ServerPktHeader(uint32 size, uint16 cmd, bool willEncrypt)
    {
        if (willEncrypt) {
            uint32 const encrypted = ((size - 2) << 13) | (uint32(cmd) & 0x1FFF);
            header[0] = 0xFF & encrypted;
            header[1] = 0xFF & (encrypted >> 8);
            header[2] = 0xFF & (encrypted >> 16);
            header[3] = 0xFF & (encrypted >> 24);
        } else {
            header[0] = 0xFF & size;
            header[1] = 0xFF & (size >> 8);
            header[2] = 0xFF & cmd;
            header[3] = 0xFF & (cmd >> 8);
        }
    }

    uint8 header[ServerPktHeader::Length];
};

struct ClientPktHeader
{
    uint16 size;
    uint16 cmd;
};

struct ClientCryptPktHeader
{
    uint32 header;
};

#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

} // namespace

WorldSocket::WorldSocket (void): WorldHandler(),
    m_LastPingTime(ACE_Time_Value::zero), m_OverSpeedPings(0), m_Session(0),
    m_RecvWPct(0), m_RecvPct(), m_Header(sizeof (ClientPktHeader)),
    m_OutBuffer(0), m_OutBufferSize(65536), m_OutActive(false),
    m_Seed(static_cast<uint32> (rand32()))
{
    reference_counting_policy().value (ACE_Event_Handler::Reference_Counting_Policy::ENABLED);

    msg_queue()->high_water_mark(8 * 1024 * 1024);
    msg_queue()->low_water_mark(8 * 1024 * 1024);
}

WorldSocket::~WorldSocket (void)
{
    delete m_RecvWPct;

    if (m_OutBuffer)
        m_OutBuffer->release();

    closing_ = true;

    peer().close();
}

bool WorldSocket::IsClosed (void) const
{
    return closing_;
}

void WorldSocket::CloseSocket (void)
{
    {
        ACE_GUARD (LockType, Guard, m_OutBufferLock);

        if (closing_)
            return;

        closing_ = true;
        peer().close_writer();
    }

    {
        ACE_GUARD (LockType, Guard, m_SessionLock);

        m_Session = NULL;
    }
}

const std::string& WorldSocket::GetRemoteAddress (void) const
{
    return m_Address;
}

int WorldSocket::SendPacket(WorldPacket const* pct)
{
    ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

    if (closing_)
        return -1;

    // Dump outgoing packet
    LogPacket(*pct, SERVER_TO_CLIENT);

    if (m_Session)
    {
        switch (pct->GetOpcode())
        {
            case SMSG_MONSTER_MOVE:
            case SMSG_UPDATE_OBJECT:
            case SMSG_THREAT_UPDATE:
                break;
            default:
                TC_LOG_ERROR("network", "S->C: %s %s",
                     m_Session->GetPlayerName().c_str(), GetOpcodeNameForLogging(pct->GetOpcode(), WOW_SERVER).c_str());
                break;
        }
    }

    ServerPktHeader header(pct->size() + 2, pct->GetOpcode(), m_Crypt.IsInitialized());
    m_Crypt.EncryptSend(header.header, ServerPktHeader::Length);

    if (m_OutBuffer->space() >= pct->size() + ServerPktHeader::Length && msg_queue()->is_empty())
    {
        // Put the packet on the buffer.
        if (m_OutBuffer->copy((char*)header.header, ServerPktHeader::Length) == -1)
            ACE_ASSERT (false);

        if (!pct->empty())
            if (m_OutBuffer->copy((char*)pct->contents(), pct->size()) == -1)
                ACE_ASSERT (false);
    }
    else
    {
        // Enqueue the packet.
        ACE_Message_Block* mb;

        ACE_NEW_RETURN(mb, ACE_Message_Block(pct->size() + ServerPktHeader::Length), -1);

        mb->copy((char*) header.header, ServerPktHeader::Length);

        if (!pct->empty())
            mb->copy((const char*)pct->contents(), pct->size());

        if (msg_queue()->enqueue_tail(mb, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
        {
            TC_LOG_ERROR("network", "WorldSocket::SendPacket enqueue_tail failed");
            mb->release();
            return -1;
        }
    }

    return 0;
}

long WorldSocket::AddReference (void)
{
    return static_cast<long> (add_reference());
}

long WorldSocket::RemoveReference (void)
{
    return static_cast<long> (remove_reference());
}

int WorldSocket::open (void *a)
{
    ACE_UNUSED_ARG (a);

    // Prevent double call to this func.
    if (m_OutBuffer)
        return -1;

    // This will also prevent the socket from being Updated
    // while we are initializing it.
    m_OutActive = true;

    // Hook for the manager.
    if (sWorldSocketMgr->OnSocketOpen(this) == -1)
        return -1;

    // Allocate the buffer.
    ACE_NEW_RETURN (m_OutBuffer, ACE_Message_Block (m_OutBufferSize), -1);

    // Store peer address.
    ACE_INET_Addr remote_addr;

    if (peer().get_remote_addr(remote_addr) == -1)
    {
        TC_LOG_ERROR("network", "WorldSocket::open: peer().get_remote_addr errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    m_Address = remote_addr.get_host_addr();

    // not an opcode. this packet sends raw string WORLD OF WARCRAFT CONNECTION - SERVER TO CLIENT"
    // because of our implementation, bytes "WO" become the opcode
    WorldPacket packet(MSG_VERIFY_CONNECTIVITY);
    packet << "RLD OF WARCRAFT CONNECTION - SERVER TO CLIENT";

    if (SendPacket(&packet) == -1)
        return -1;

    // Register with ACE Reactor
    if (reactor()->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK) == -1)
    {
        TC_LOG_ERROR("network", "WorldSocket::open: unable to register client handler errno = %s", ACE_OS::strerror (errno));
        return -1;
    }

    // reactor takes care of the socket from now on
    remove_reference();

    return 0;
}

int WorldSocket::close (u_long)
{
    shutdown();

    closing_ = true;

    remove_reference();

    return 0;
}

int WorldSocket::handle_input (ACE_HANDLE)
{
    if (closing_)
        return -1;

    switch (handle_input_missing_data())
    {
    case -1 :
        {
            if ((errno == EWOULDBLOCK) ||
                (errno == EAGAIN))
            {
                return Update();                           // interesting line, isn't it ?
            }

            TC_LOG_DEBUG("network", "WorldSocket::handle_input: Peer error closing connection errno = %s", ACE_OS::strerror (errno));

            errno = ECONNRESET;
            return -1;
        }
    case 0:
        {
            TC_LOG_DEBUG("network", "WorldSocket::handle_input: Peer has closed connection");

            errno = ECONNRESET;
            return -1;
        }
    case 1:
        return 1;
    default:
        return Update();                               // another interesting line ;)
    }

    ACE_NOTREACHED(return -1);
}

int WorldSocket::handle_output (ACE_HANDLE)
{
    ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

    if (closing_)
        return -1;

    size_t send_len = m_OutBuffer->length();

    if (send_len == 0)
        return handle_output_queue(Guard);

#ifdef MSG_NOSIGNAL
    ssize_t n = peer().send (m_OutBuffer->rd_ptr(), send_len, MSG_NOSIGNAL);
#else
    ssize_t n = peer().send (m_OutBuffer->rd_ptr(), send_len);
#endif // MSG_NOSIGNAL

    if (n == 0)
        return -1;
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
            return schedule_wakeup_output (Guard);

        return -1;
    }
    else if (n < (ssize_t)send_len) //now n > 0
    {
        m_OutBuffer->rd_ptr (static_cast<size_t> (n));

        // move the data to the base of the buffer
        m_OutBuffer->crunch();

        return schedule_wakeup_output (Guard);
    }
    else //now n == send_len
    {
        m_OutBuffer->reset();

        return handle_output_queue (Guard);
    }

    ACE_NOTREACHED (return 0);
}

int WorldSocket::handle_output_queue (GuardType& g)
{
    if (msg_queue()->is_empty())
        return cancel_wakeup_output(g);

    ACE_Message_Block* mblk;

    if (msg_queue()->dequeue_head(mblk, (ACE_Time_Value*)&ACE_Time_Value::zero) == -1)
    {
        TC_LOG_ERROR("network", "WorldSocket::handle_output_queue dequeue_head");
        return -1;
    }

    const size_t send_len = mblk->length();

#ifdef MSG_NOSIGNAL
    ssize_t n = peer().send (mblk->rd_ptr(), send_len, MSG_NOSIGNAL);
#else
    ssize_t n = peer().send (mblk->rd_ptr(), send_len);
#endif // MSG_NOSIGNAL

    if (n == 0)
    {
        mblk->release();

        return -1;
    }
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        {
            msg_queue()->enqueue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero);
            return schedule_wakeup_output (g);
        }

        mblk->release();
        return -1;
    }
    else if (n < (ssize_t)send_len) //now n > 0
    {
        mblk->rd_ptr(static_cast<size_t> (n));

        if (msg_queue()->enqueue_head(mblk, (ACE_Time_Value*) &ACE_Time_Value::zero) == -1)
        {
            TC_LOG_ERROR("network", "WorldSocket::handle_output_queue enqueue_head");
            mblk->release();
            return -1;
        }

        return schedule_wakeup_output (g);
    }
    else //now n == send_len
    {
        mblk->release();

        return msg_queue()->is_empty() ? cancel_wakeup_output(g) : ACE_Event_Handler::WRITE_MASK;
    }

    ACE_NOTREACHED(return -1);
}

int WorldSocket::handle_close (ACE_HANDLE h, ACE_Reactor_Mask)
{
    // Critical section
    {
        ACE_GUARD_RETURN (LockType, Guard, m_OutBufferLock, -1);

        closing_ = true;

        if (h == ACE_INVALID_HANDLE)
            peer().close_writer();
    }

    // Critical section
    {
        ACE_GUARD_RETURN (LockType, Guard, m_SessionLock, -1);

        m_Session = NULL;
    }

    reactor()->remove_handler(this, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::ALL_EVENTS_MASK);
    return 0;
}

int WorldSocket::Update (void)
{
    if (closing_)
        return -1;

    if (m_OutActive)
        return 0;

    {
        ACE_GUARD_RETURN(LockType, Guard, m_OutBufferLock, 0);
        if (m_OutBuffer->length() == 0 && msg_queue()->is_empty())
            return 0;
    }

    int ret;
    do
    ret = handle_output(get_handle());
    while (ret > 0);

    return ret;
}

int WorldSocket::handle_input_header (void)
{
    ACE_ASSERT(m_RecvWPct == NULL);

    ACE_ASSERT(m_Header.length() == sizeof(ClientPktHeader));

    m_Crypt.DecryptRecv ((uint8*)m_Header.rd_ptr(), sizeof(ClientPktHeader));

    uint16 cmd = 0;
    uint16 size = 0;

    if (m_Crypt.IsInitialized())
    {
        ClientCryptPktHeader& header = *((ClientCryptPktHeader*)m_Header.rd_ptr());
        size = (uint16)(header.header >> 13);
        cmd = (uint16)(header.header & 0x1FFF);
    }
    else
    {
        ClientPktHeader& header = *((ClientPktHeader*)m_Header.rd_ptr());
        cmd = header.cmd;
        size = header.size;
    }

    if (size > 10240)  // LR (from MSG_VERIFY_CONNECTIVITY)
    {
        Player* _player = m_Session ? m_Session->GetPlayer() : NULL;
        TC_LOG_ERROR("network", "WorldSocket::handle_input_header(): client (account: %u, char [GUID: %u, name: %s])"
                       " sent malformed packet (size: %d, cmd: %d)",
                       m_Session ? m_Session->GetAccountId() : 0,
                       _player ? _player->GetGUIDLow() : 0,
                       _player ? _player->GetName().c_str() : "<none>",
                       size, cmd);

        errno = EINVAL;
        return -1;
    }


    if (!m_Crypt.IsInitialized())
        size -= 2;

    ACE_NEW_RETURN(m_RecvWPct, WorldPacket (DropHighBytes(Opcodes(cmd)), size), -1);

    if (size > 0)
    {
        m_RecvWPct->resize(size);
        m_RecvPct.base ((char*) m_RecvWPct->contents(), m_RecvWPct->size());
    }
    else
    {
        ACE_ASSERT(m_RecvPct.space() == 0);
    }

    return 0;
}

int WorldSocket::handle_input_payload (void)
{
    // set errno properly here on error !!!
    // now have a header and payload

    ACE_ASSERT (m_RecvPct.space() == 0);
    ACE_ASSERT (m_Header.space() == 0);
    ACE_ASSERT (m_RecvWPct != NULL);

    const int ret = ProcessIncoming (m_RecvWPct);

    m_RecvPct.base (NULL, 0);
    m_RecvPct.reset();
    m_RecvWPct = NULL;

    m_Header.reset();

    if (ret == -1)
        errno = EINVAL;

    return ret;
}

int WorldSocket::handle_input_missing_data (void)
{
    char buf [4096];

    ACE_Data_Block db (sizeof (buf),
        ACE_Message_Block::MB_DATA,
        buf,
        0,
        0,
        ACE_Message_Block::DONT_DELETE,
        0);

    ACE_Message_Block message_block(&db,
        ACE_Message_Block::DONT_DELETE,
        0);

    const size_t recv_size = message_block.space();

    const ssize_t n = peer().recv (message_block.wr_ptr(),
        recv_size);

    if (n <= 0)
        return int(n);

    message_block.wr_ptr (n);

    while (message_block.length() > 0)
    {
        if (m_Header.space() > 0)
        {
            //need to receive the header
            const size_t to_header = (message_block.length() > m_Header.space() ? m_Header.space() : message_block.length());
            m_Header.copy (message_block.rd_ptr(), to_header);
            message_block.rd_ptr (to_header);

            if (m_Header.space() > 0)
            {
                // Couldn't receive the whole header this time.
                ACE_ASSERT (message_block.length() == 0);
                errno = EWOULDBLOCK;
                return -1;
            }

            // We just received nice new header
            if (handle_input_header() == -1)
            {
                ACE_ASSERT ((errno != EWOULDBLOCK) && (errno != EAGAIN));
                return -1;
            }
        }

        // Its possible on some error situations that this happens
        // for example on closing when epoll receives more chunked data and stuff
        // hope this is not hack, as proper m_RecvWPct is asserted around
        if (!m_RecvWPct)
        {
            TC_LOG_ERROR("network", "Forcing close on input m_RecvWPct = NULL");
            errno = EINVAL;
            return -1;
        }

        // We have full read header, now check the data payload
        if (m_RecvPct.space() > 0)
        {
            //need more data in the payload
            const size_t to_data = (message_block.length() > m_RecvPct.space() ? m_RecvPct.space() : message_block.length());
            m_RecvPct.copy (message_block.rd_ptr(), to_data);
            message_block.rd_ptr (to_data);

            if (m_RecvPct.space() > 0)
            {
                // Couldn't receive the whole data this time.
                ACE_ASSERT (message_block.length() == 0);
                errno = EWOULDBLOCK;
                return -1;
            }
        }

        //just received fresh new payload
        if (handle_input_payload() == -1)
        {
            ACE_ASSERT ((errno != EWOULDBLOCK) && (errno != EAGAIN));
            return -1;
        }
    }

    return size_t(n) == recv_size ? 1 : 2;
}

int WorldSocket::cancel_wakeup_output (GuardType& g)
{
    if (!m_OutActive)
        return 0;

    m_OutActive = false;

    g.release();

    if (reactor()->cancel_wakeup
        (this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        // would be good to store errno from reactor with errno guard
        TC_LOG_ERROR("network", "WorldSocket::cancel_wakeup_output");
        return -1;
    }

    return 0;
}

int WorldSocket::schedule_wakeup_output (GuardType& g)
{
    if (m_OutActive)
        return 0;

    m_OutActive = true;

    g.release();

    if (reactor()->schedule_wakeup
        (this, ACE_Event_Handler::WRITE_MASK) == -1)
    {
        TC_LOG_ERROR("network", "WorldSocket::schedule_wakeup_output");
        return -1;
    }

    return 0;
}

int WorldSocket::ProcessIncoming(WorldPacket* new_pct)
{
    ACE_ASSERT (new_pct);

    // manage memory ;)
    ACE_Auto_Ptr<WorldPacket> aptr(new_pct);

    Opcodes opcode = new_pct->GetOpcode();

    if (closing_)
        return -1;

    LogPacket(*new_pct, CLIENT_TO_SERVER);

    if (m_Session)
    {
        switch (opcode)
        {
            case CMSG_PLAYER_MOVE:
            case CMSG_MOVE_TIME_SKIPPED:
                break;
            default:
                TC_LOG_ERROR("network", "C->S: %s %s",
                     m_Session->GetPlayerName().c_str(), GetOpcodeNameForLogging(opcode, WOW_CLIENT).c_str());
                break;
        }
    }

    try
    {
        switch (opcode)
        {
            case CMSG_PING:
                return HandlePing(*new_pct);
            case CMSG_AUTH_SESSION:
            {
                if (m_Session)
                {
                    TC_LOG_ERROR("network", "WorldSocket::ProcessIncoming: received duplicate CMSG_AUTH_SESSION from %s", m_Session->GetPlayerName(false).c_str());
                    return -1;
                }

                return HandleAuthSession(*new_pct);
            }
            case CMSG_KEEP_ALIVE:
            {
                TC_LOG_DEBUG("network", "CMSG_KEEP_ALIVE");
                return 0;
            }
            case CMSG_LOG_DISCONNECT:
            {
                new_pct->rfinish(); // contains uint32 disconnectReason;
                TC_LOG_DEBUG("network", "CMSG_LOG_DISCONNECT");
                return 0;
            }
            /*case CMSG_REORDER_CHARACTERS:
            {
                if (m_Session)
                    if (OpcodeHandler* opHandle = opcodeTable[CMSG_REORDER_CHARACTERS])
                        (m_Session->*opHandle->handler)(*new_pct);

                return 0;
            }*/
            // not an opcode, client sends string "WORLD OF WARCRAFT CONNECTION - CLIENT TO SERVER" without opcode
            // first 4 bytes become the opcode (2 dropped)
            case MSG_VERIFY_CONNECTIVITY:
            {
                TC_LOG_DEBUG("network", "MSG_VERIFY_CONNECTIVITY");
                std::string str;
                *new_pct >> str;
                if (str != "RLD OF WARCRAFT CONNECTION - CLIENT TO SERVER")
                    return -1;
                return HandleSendAuthSession();
            }
            /*case CMSG_ENABLE_NAGLE:
            {
                TC_LOG_DEBUG("network", "CMSG_ENABLE_NAGLE");
                return m_Session ? m_Session->HandleEnableNagleAlgorithm() : -1;
            }*/
            default:
            {
                ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);
                if (!m_Session)
                {
                    TC_LOG_ERROR("network.opcode", "ProcessIncoming: Client not authed opcode = %u", uint32(opcode));
                    return -1;
                }

                // prevent invalid memory access/crash with custom opcodes
                if (opcode >= NUM_OPCODE_HANDLERS)
                    return 0;

                OpcodeHandler* handler = opcodeTable[WOW_CLIENT][opcode];
                if (!handler || handler->status == STATUS_UNHANDLED)
                {
                    //TC_LOG_ERROR("network.opcode", "No defined handler for opcode 0x%04X sent by player %s, account %u",
                                 //new_pct->GetOpcode(), m_Session->GetPlayerName().c_str(), m_Session->GetAccountId());
                    return 0;
                }

                // Our Idle timer will reset on any non PING opcodes.
                // Catches people idling on the login screen and any lingering ingame connections.
                m_Session->ResetTimeOutTime();

                // OK, give the packet to WorldSession
                aptr.release();
                // WARNING here we call it with locks held.
                // Its possible to cause deadlock if QueuePacket calls back
                m_Session->QueuePacket(new_pct);
                return 0;
            }
        }
    }
    catch (ByteBufferException &)
    {
        TC_LOG_ERROR("network", "WorldSocket::ProcessIncoming ByteBufferException occured while parsing an instant"
                     " handled packet 0x%04X from client %s, accountid=%i. Disconnected client.",
                     new_pct->GetOpcode(), GetRemoteAddress().c_str(), m_Session ? int32(m_Session->GetAccountId()) : -1);
        TC_LOG_TRACE("network", "%s", new_pct->hexlike().c_str());
        return -1;
    }

    ACE_NOTREACHED (return 0);
}

int WorldSocket::HandleSendAuthSession()
{
    WorldPacket packet(SMSG_AUTH_CHALLENGE, 37);
    packet << uint16(0);

    for (int i = 0; i < 8; i++)
        packet << uint32(0);

    packet << uint8(1);
    packet << m_Seed;

    return SendPacket(&packet);
}

void WorldSocket::SendAuthResponse(uint8 code, bool queued, uint32 queuePos)
{
    WorldPacket packet(SMSG_AUTH_RESPONSE);

    bool hasAccountData = true;
    uint32 realmRaceCount = 15;
    uint32 realmClassCount = 11;

    packet.WriteBit(queued);
    packet.WriteBit(hasAccountData);

    if (hasAccountData)
    {
        packet.WriteBits(0, 21);
        packet.WriteBit(0);
        packet.WriteBits(realmClassCount, 23);
        packet.WriteBit(0);
        packet.WriteBit(0);

        packet.WriteBits(0, 21);
        packet.WriteBit(0);
        packet.WriteBits(realmRaceCount, 23);
    }

    if (queued)
    {
        packet.WriteBit(0);
        packet << uint32(queuePos);
    }

    if (hasAccountData)
    {
        for (uint32 i = 0; i < realmClassCount; i++)
        {
            switch (i)
            {
                case 0:
                    packet << uint8(CLASS_WARRIOR);
                    packet << uint8(0); // Prebc
                    break;
                case 1:
                    packet << uint8(CLASS_PALADIN);
                    packet << uint8(0); // Prebc
                    break;
                case 2:
                    packet << uint8(CLASS_HUNTER);
                    packet << uint8(0); // Prebc
                    break;
                case 3:
                    packet << uint8(CLASS_ROGUE);
                    packet << uint8(0); // Prebc
                    break;
                case 4:
                    packet << uint8(CLASS_PRIEST);
                    packet << uint8(0); // Prebc
                    break;
                case 5:
                    packet << uint8(CLASS_DEATH_KNIGHT);
                    packet << uint8(2); // Wotlk
                    break;
                case 6:
                    packet << uint8(CLASS_SHAMAN);
                    packet << uint8(0); // Prebc
                    break;
                case 7:
                    packet << uint8(CLASS_MAGE);
                    packet << uint8(0); // Prebc
                    break;
                case 8:
                    packet << uint8(CLASS_WARLOCK);
                    packet << uint8(0); // Prebc
                    break;
                case 9:
                    packet << uint8(CLASS_DRUID);
                    packet << uint8(0); // Prebc
                    break;
                case 10:
                    packet << uint8(CLASS_MONK);
                    packet << uint8(4); // MoP
                    break;
            }
        }

        packet << uint32(0);

        packet << uint8(0);
        packet << uint8(RACE_HUMAN);
        packet << uint8(0);
        packet << uint8(RACE_ORC);
        packet << uint8(0);
        packet << uint8(RACE_DWARF);
        packet << uint8(0);
        packet << uint8(RACE_NIGHTELF);
        packet << uint8(0);
        packet << uint8(RACE_UNDEAD_PLAYER);
        packet << uint8(0);
        packet << uint8(RACE_TAUREN);
        packet << uint8(0);
        packet << uint8(RACE_GNOME);
        packet << uint8(0);
        packet << uint8(RACE_TROLL);
        packet << uint8(1);
        packet << uint8(RACE_GOBLIN);
        packet << uint8(1);
        packet << uint8(RACE_BLOODELF);
        packet << uint8(1);
        packet << uint8(RACE_DRAENEI);
        packet << uint8(1);
        packet << uint8(RACE_WORGEN);
        packet << uint8(1);
        packet << uint8(RACE_PANDAREN_NEUTRAL);
        packet << uint8(1);
        packet << uint8(RACE_PANDAREN_ALLI);
        packet << uint8(1);
        packet << uint8(RACE_PANDAREN_HORDE);

        packet << uint32(0);
        packet << uint32(0);

        packet << uint8(Expansion());
        packet << uint8(Expansion());

        packet << uint32(0);
        packet << uint32(0);
    }

    packet << uint8(code);

    SendPacket(&packet);
}

int WorldSocket::HandleAuthSession(WorldPacket& recvPacket)
{
    uint8 digest[20], unk1, unk2;
    uint32 clientSeed, unk3, unk4, unk5, unk6;
    uint64 unk7;
    uint16 clientBuild, security;
    uint32 id;
    uint32 addonSize;
    LocaleConstant locale;
    std::string account;
    SHA1Hash sha;
    BigNumber k;
    WorldPacket addonsData;

    recvPacket.read_skip<uint16>();
    recvPacket >> unk3;
    recvPacket >> digest[4];
    recvPacket >> unk4;
    recvPacket >> unk1;
    recvPacket >> digest[19];
    recvPacket >> digest[12];
    recvPacket >> digest[9];
    recvPacket >> digest[6];
    recvPacket >> digest[18];
    recvPacket >> digest[17];
    recvPacket >> digest[8];
    recvPacket >> digest[13];
    recvPacket >> digest[1];
    recvPacket >> digest[10];
    recvPacket >> digest[11];
    recvPacket >> digest[15];
    recvPacket >> clientSeed;
    recvPacket >> digest[3];
    recvPacket >> digest[14];
    recvPacket >> digest[7];
    recvPacket >> unk7;
    recvPacket >> unk2;
    recvPacket >> unk5;
    recvPacket >> digest[5];
    recvPacket >> digest[0];
    recvPacket >> clientBuild;
    recvPacket >> digest[16];
    recvPacket >> digest[2];
    recvPacket >> unk6;

    recvPacket >> addonSize;
    addonsData.resize(addonSize);
    recvPacket.read(addonsData.contents(), addonSize);

    uint32 accountNameLength = recvPacket.ReadBits(11);
    recvPacket.ReadBit();
    account = recvPacket.ReadString(accountNameLength);

    if (sWorld->IsClosed())
    {
        SendAuthResponse(AUTH_REJECT, false, 0);
        TC_LOG_ERROR("network", "WorldSocket::HandleAuthSession: World closed, denying client (%s).", GetRemoteAddress().c_str());
        return -1;
    }

    // Get the account information from the realmd database
    std::string safe_account = account; // Duplicate, else will screw the SHA hash verification below
    LoginDatabase.EscapeString (safe_account);
    // No SQL injection, username escaped.

    //                                                 0       1          2       3     4  5      6          7       8         9      10
    QueryResult result = LoginDatabase.PQuery ("SELECT id, sessionkey, last_ip, locked, v, s, expansion, mutetime, locale, recruiter, os FROM account "
                                               "WHERE username = '%s'", safe_account.c_str());

    // Stop if the account is not found
    if (!result)
    {
        SendAuthResponse(AUTH_UNKNOWN_ACCOUNT, false, 0);
        TC_LOG_ERROR("network", "WorldSocket::HandleAuthSession: Sent Auth Response (unknown account).");
        return -1;
    }

    Field* fields = result->Fetch();

    uint8 expansion = fields[6].GetUInt8();
    uint32 world_expansion = sWorld->getIntConfig(CONFIG_EXPANSION);
    if (expansion > world_expansion)
        expansion = world_expansion;

    TC_LOG_DEBUG("network", "WorldSocket::HandleAuthSession: (s,v) check s: %s v: %s",
        fields[5].GetCString(),
        fields[4].GetCString());

    ///- Re-check ip locking (same check as in realmd).
    if (fields[3].GetUInt8() == 1) // if ip is locked
    {
        if (strcmp (fields[2].GetCString(), GetRemoteAddress().c_str()))
        {
            SendAuthResponse(AUTH_FAILED, false, 0);
            TC_LOG_DEBUG("network", "WorldSocket::HandleAuthSession: Sent Auth Response (Account IP differs).");
            return -1;
        }
    }

    id = fields[0].GetUInt32();
    /*
    if (security > SEC_ADMINISTRATOR)                        // prevent invalid security settings in DB
    security = SEC_ADMINISTRATOR;
    */

    k.SetHexStr(fields[1].GetCString());

    int64 mutetime = fields[7].GetInt64();
    //! Negative mutetime indicates amount of seconds to be muted effective on next login - which is now.
    if (mutetime < 0)
    {
        mutetime = time(NULL) + llabs(mutetime);

        PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_MUTE_TIME_LOGIN);

        stmt->setInt64(0, mutetime);
        stmt->setUInt32(1, id);

        LoginDatabase.Execute(stmt);
    }

    locale = LocaleConstant (fields[8].GetUInt8());
    if (locale >= TOTAL_LOCALES)
        locale = LOCALE_enUS;

    uint32 recruiter = fields[9].GetUInt32();
    std::string os = fields[10].GetString();

    // Checks gmlevel per Realm
    result =
        LoginDatabase.PQuery ("SELECT "
                              "RealmID, "            //0
                              "gmlevel "             //1
                              "FROM account_access "
                              "WHERE id = '%d'"
                              " AND (RealmID = '%d'"
                              " OR RealmID = '-1')",
                              id, realmID);

    if (!result)
        security = 0;
    else
    {
        fields = result->Fetch();
        security = fields[1].GetInt32();
    }

    // Re-check account ban (same check as in realmd)
    QueryResult banresult =
          LoginDatabase.PQuery ("SELECT 1 FROM account_banned WHERE id = %u AND active = 1 "
                                "UNION "
                                "SELECT 1 FROM ip_banned WHERE ip = '%s'",
                                id, GetRemoteAddress().c_str());

    if (banresult) // if account banned
    {
        SendAuthResponse(AUTH_BANNED, false, 0);
        TC_LOG_ERROR("network", "WorldSocket::HandleAuthSession: Sent Auth Response (Account banned).");
        return -1;
    }

    // Check locked state for server
    AccountTypes allowedAccountType = sWorld->GetPlayerSecurityLimit();
    TC_LOG_DEBUG("network", "Allowed Level: %u Player Level %u", allowedAccountType, AccountTypes(security));
    if (allowedAccountType > SEC_PLAYER && AccountTypes(security) < allowedAccountType)
    {
        SendAuthResponse(AUTH_UNAVAILABLE, false, 0);
        TC_LOG_INFO("network", "WorldSocket::HandleAuthSession: User tries to login but his security level is not enough");
        return -1;
    }

    // Check that Key and account name are the same on client and server
    uint32 t = 0;
    uint32 seed = m_Seed;

    sha.UpdateData(account);
    sha.UpdateData((uint8*)&t, 4);
    sha.UpdateData((uint8*)&clientSeed, 4);
    sha.UpdateData((uint8*)&seed, 4);
    sha.UpdateBigNumbers(&k, NULL);
    sha.Finalize();

    std::string address = GetRemoteAddress();

    /*if (memcmp(sha.GetDigest(), digest, 20))
    {
    WorldPacket packet(SMSG_AUTH_RESPONSE, 1);
    packet.WriteBit(0); // has queue info
    packet.WriteBit(0); // has account info
    packet << uint8(AUTH_FAILED);
    SendPacket(packet);

    TC_LOG_ERROR("network", "WorldSocket::HandleAuthSession: Authentication failed for account: %u ('%s') address: %s", id, account.c_str(), address.c_str());
    return -1;
    }*/

    TC_LOG_DEBUG("network", "WorldSocket::HandleAuthSession: Client '%s' authenticated successfully from %s.",
        account.c_str(),
        address.c_str());

    bool isRecruiter = false;

    // Update the last_ip in the database
    // No SQL injection, username escaped.
    LoginDatabase.EscapeString (address);

    LoginDatabase.PExecute ("UPDATE account "
                            "SET last_ip = '%s' "
                            "WHERE username = '%s'",
                            address.c_str(),
                            safe_account.c_str());

    // NOTE ATM the socket is single-threaded, have this in mind ...
    ACE_NEW_RETURN(m_Session, WorldSession(id, this, AccountTypes(security), expansion, mutetime, locale, recruiter, isRecruiter), -1);

    m_Crypt.Init(&k);

    m_Session->LoadGlobalAccountData();
    m_Session->LoadTutorialsData();
    m_Session->ReadAddonsInfo(addonsData);

    // Initialize Warden system only if it is enabled by config
    if (sWorld->getBoolConfig(CONFIG_WARDEN_ENABLED))
        m_Session->InitWarden(&k, os);

    sWorld->AddSession(m_Session);
    return 0;
}

int WorldSocket::HandlePing(WorldPacket& recvPacket)
{
    uint32 ping;
    uint32 latency;

    // Get the ping packet content
    recvPacket >> ping;
    recvPacket >> latency;

    if (m_LastPingTime == ACE_Time_Value::zero)
        m_LastPingTime = ACE_OS::gettimeofday(); // for 1st ping
    else
    {
        ACE_Time_Value cur_time = ACE_OS::gettimeofday();
        ACE_Time_Value diff_time(cur_time);
        diff_time -= m_LastPingTime;
        m_LastPingTime = cur_time;

        if (diff_time < ACE_Time_Value(27))
        {
            ++m_OverSpeedPings;

            uint32 max_count = sWorld->getIntConfig(CONFIG_MAX_OVERSPEED_PINGS);

            if (max_count && m_OverSpeedPings > max_count)
            {
                ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);

                if (m_Session && AccountMgr::IsPlayerAccount(m_Session->GetSecurity()))
                {
                    TC_LOG_ERROR("network", "WorldSocket::HandlePing: %s kicked for over-speed pings (address: %s)",
                        m_Session->GetPlayerName(false).c_str(), GetRemoteAddress().c_str());

                    return -1;
                }
            }
        }
        else
            m_OverSpeedPings = 0;
    }

    // critical section
    {
        ACE_GUARD_RETURN(LockType, Guard, m_SessionLock, -1);

        if (m_Session)
            m_Session->SetLatency (latency);
        else
        {
            TC_LOG_ERROR("network", "WorldSocket::HandlePing: peer sent CMSG_PING, "
                "but is not authenticated or got recently kicked, "
                " address = %s",
                GetRemoteAddress().c_str());
            return -1;
        }
    }

    WorldPacket packet(SMSG_PONG, 4);
    packet << ping;
    return SendPacket(&packet);
}

void WorldSocket::LogPacket(WorldPacket const &packet, Direction direction)
{
    if (m_Session)
    {
        // This permission can be enabled at runtime, so we perform
        // lazy initialization
        if (!m_packetLog.CanLogPacket())
        {
            std::string accountName;
            AccountMgr::GetName(m_Session->GetAccountId(), accountName);

            std::ostringstream ss;
            ss << accountName << '_'
               << TimeToTimestampStr(std::time(NULL)) << ".bin";

            m_packetLog.Initialize(ss.str());
        }

        m_packetLog.LogPacket(packet, direction);
    }
}
