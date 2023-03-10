/*
# Modifications by Gavin
- v2017.11.11 - Added module announce, Updated strings

---

 *  Originally written  for TrinityCore by ShinDarth and GigaDev90 (www.trinitycore.org)
 *  Converted as module for AzerothCore by ShinDarth and Yehonal   (www.azerothcore.org)
 *  Modifications by Gavin - v2017.11.11
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mod_FakePlayer.h"
#include "PlayerBotAI.h"

using namespace lfg;

bool FakePlayerMgr::HandleTeleportAck(Player* bot)
{
    bot->GetMotionMaster()->Clear();
    if (bot->IsBeingTeleportedNear())
    {
        //WorldPacket* data = new WorldPacket(MSG_MOVE_TELEPORT_ACK, 8 + 4 + 4);
        //*data << bot->GetPackGUID();
        //*data << uint32(0);       // supposed to be flags? not used currently
        //*data << uint32(time(0)); // time - not currently used
        //bot->GetSession()->QueuePacket(data);

        WorldPacket data(MSG_MOVE_TELEPORT_ACK, 8 + 4 + 4);
        data << bot->GetPackGUID();
        data << uint32(0);       // supposed to be flags? not used currently
        data << uint32(time(0)); // time - not currently used
        bot->GetSession()->HandleMoveTeleportAck(data);
    }
    else if (bot->IsBeingTeleportedFar())
    {
        //WorldPacket* data = new WorldPacket(MSG_MOVE_WORLDPORT_ACK, 0);
        //bot->GetSession()->QueuePacket(data);

        //WorldPacket data;
        //bot->GetSession()->HandleMoveWorldportAckOpcode(data);
        bot->GetSession()->HandleMoveWorldportAck();
    }

    //SwitchPlayerBotAI(bot);
    return true;
}

bool FakePlayerMgr::HandleGroupInvite(Player* bot, WorldPacket& packet)
{
    std::string membername;
    packet.read_skip<uint8>();
    packet >> membername;

    Player* invitedPlayer = ObjectAccessor::FindPlayerByName(membername);
    if (!invitedPlayer)
        return false;

    Group* grp = bot->GetGroupInvite();
    if (!grp)
        return false;

    if (invitedPlayer->GetSession()->GetSecurity() == SEC_PLAYER)
    {
        sFakePlayerMgr->TaskSafe(1s, 10s, bot, [](Player* player)
        {
            WorldPacket data;
            player->GetSession()->HandleGroupDeclineOpcode(data);
        });
        return false;
    }

    //Player* inviter = ObjectAccessor::FindPlayer(grp->GetLeaderGUID());
    //if (!inviter)
    //    return;

    WorldPacket data;
    data << uint32(0);
    bot->GetSession()->HandleGroupAcceptOpcode(data);

    if (grp->isRaidGroup())
    {
        WorldPacket data;
        ChatHandler::BuildChatPacket(data, CHAT_MSG_RAID, Language(LANG_UNIVERSAL), bot, nullptr, "大家好啊");
        grp->BroadcastPacket(&data, false);
    }
    else
    {
        WorldPacket data;
        ChatHandler::BuildChatPacket(data, CHAT_MSG_PARTY, Language(LANG_UNIVERSAL), bot, nullptr, "大家好啊");
        grp->BroadcastPacket(&data, false, grp->GetMemberGroup(bot->GetGUID()));
    }

    //SwitchPlayerBotAI(bot);
    SelectPlayerBotAI(bot, AITYPE_GROUP);
    return true;
}

bool FakePlayerMgr::HandleResurrect(Player* bot, WorldPacket& packet)
{
    if (bot->IsAlive())
        return false;

    packet.rpos(0);
    uint64 guid;
    packet >> guid;

    WorldPacket* const data = new WorldPacket(CMSG_RESURRECT_RESPONSE, 8 + 1);
    *data << guid;
    *data << uint8(1);                        // accept
    bot->GetSession()->QueuePacket(data);     // queue the packet to get around race condition

    return true;
}

bool FakePlayerMgr::HandleLFGJoinResult(Player* bot, WorldPacket& packet)
{
    uint32 result;
    packet >> result;

    if (result != LFG_JOIN_OK)
        m_QueueLfgCooldownStore.insert(bot->GetGUID());

    return true;
}

//bool FakePlayerMgr::HandleLFGRoleChosen(Player* bot, WorldPacket& packet)
//{
//    ObjectGuid sendGuid;
//    packet >> sendGuid;
//
//    ObjectGuid botGuid = bot->GetGUID();
//    if (sendGuid == botGuid)
//        return false;
//
//    if (Group* group = bot->GetGroup())
//    {
//        uint8 currentRoles = sLFGMgr->GetRoles(botGuid);
//        uint8 newRoles = GenerateLfgRoles(bot);
//        if (currentRoles == newRoles) return false;
//
//        //sLFGMgr->UpdateRoleCheck(group->GetGUID(), botGuid, newRoles);
//        // 不要立即处理, 否则会导致UpdateRoleCheck异常
//        sLFGMgr->SetRoles(botGuid, newRoles);
//        WorldPacket* const data = new WorldPacket(CMSG_LFG_SET_ROLES, 1);
//        *data << newRoles;
//        bot->GetSession()->QueuePacket(data);
//    }
//
//    return true;
//}

bool FakePlayerMgr::HandleLFGRoleCheck(Player* bot, WorldPacket& packet)
{
#ifndef TBC243
    uint8 dungeonSize, roleSize;
    packet.read_skip(5);
    packet >> dungeonSize;
    if(dungeonSize)
        packet.read_skip(dungeonSize * 4);
    packet >> roleSize;

    ObjectGuid nowGuid;
    uint8 ready;
    ObjectGuid botGuid = bot->GetGUID();
    for (uint8 i = 0; i < roleSize; ++i)
    {
        packet >> nowGuid >> ready;
        if (nowGuid == botGuid)
        {
            if (ready)
                return false;

            break;
        }
        packet.read_skip(5);
    }

    if (Group* group = bot->GetGroup())
    {
        uint8 newRoles = GenerateLfgRoles(bot);
        sFakePlayerMgr->TaskOneSafe(1s, 5s, EVENT_HANDLE_LFG_ROLE_CHECK, bot, [group, botGuid, newRoles](Player* player)
        {
            WorldPacket* const data = new WorldPacket(CMSG_LFG_SET_ROLES, 1);
            *data << newRoles;
            player->GetSession()->QueuePacket(data);
            //sLFGMgr->UpdateRoleCheck(group->GetGUID(), botGuid, newRoles);
        });
    }
#endif

    return true;
}

bool FakePlayerMgr::HandleLFGProposal(Player* bot, WorldPacket& packet)
{
#ifndef TBC243
    if (!bot->isUsingLfg())
        return false;

    //uint32 dungeon;
    //uint8 state;
    uint32 proposalId;
    uint8 groupSize;
    //packet >> dungeon >> state >> proposalId;
    packet.read_skip(5);
    packet >> proposalId;
    packet.read_skip(5);
    packet >> groupSize;

    uint8 self = false;
    uint8 answer = false;
    uint8 accept = false;
    for (uint8 i = 0; i < groupSize; ++i)
    {
        packet.read_skip(4);
        packet >> self;
        if (self)
        {
            packet.read_skip(2);
            packet >> answer >> accept;
            if (answer/* || accept*/)
                return false;

            break;
        }
        else
            packet.read_skip(4);
    }

    //sLFGMgr->UpdateProposal(proposalId, bot->GetGUID(), true);
    sFakePlayerMgr->TaskOneSafe(1s, 5s, EVENT_HANDLE_LFG_PROPOSAL, bot, [this, proposalId](Player* player)
    {
        WorldPacket* const data = new WorldPacket(CMSG_LFG_PROPOSAL_RESULT, 4 + 1);
        *data << proposalId;
        *data << uint8(1);
        player->GetSession()->QueuePacket(data);
        //sLFGMgr->UpdateProposal(proposalId, player->GetGUID(), true);
        //SelectPlayerBotAI(player, AITYPE_GROUP);
    });
#endif

    return true;
}

bool FakePlayerMgr::HandleDuelRequested(Player* bot, WorldPacket& packet)
{
    //ObjectGuid flagGuid, playerGuid;
    //packet >> flagGuid >> playerGuid;
    uint64 flagGuid;
    packet >> flagGuid;

    WorldPacket* const data = new WorldPacket(CMSG_DUEL_ACCEPTED, 8);
    *data << flagGuid;
    bot->GetSession()->QueuePacket(data);

    SelectPlayerBotAI(bot, AITYPE_DUEL);
    return true;
}
