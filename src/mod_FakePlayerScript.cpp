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
#include "PlayerBotSetting.h"
#include "PlayerBotBattlegroundMgr.h"
#include "PlayerBotAI.h"
#include "BotFieldAI.h"

class FakePlayerPlayerScript : public PlayerScript
{
public:
    FakePlayerPlayerScript() : PlayerScript("FakePlayerPlayerScript") { }

    //void OnLogin(Player* player, bool /*firstLogin*/) override
    //{
    //    //if (sFakePlayerMgr->Enable && sFakePlayerMgr->LoginMessage)
    //    //    ChatHandler(player->GetSession()).PSendSysMessage(GBK_TO_UTF8("此服务器正在运行 |cff4CFF00真实假人 |r模块."));
    //}

    void OnLogout(Player* player) override
    {
        sFakePlayerMgr->RemovePlayerBot(player->GetGUID());
    }

    bool OnChat(Player* player, uint32 /*type*/, uint32 lang, std::string& msg, Player* receiver) override
    {
        if (!sFakePlayerMgr->Enable || !sFakePlayerMgr->MessageToGameMaster)
            return false;

        if(sFakePlayerMgr->IsBotPlayer(receiver))
        {
            // 插件消息, 不需要任何处理
            if (lang == LANG_ADDON)
                return true;

            std::string message("|cffff0000[真实假人]|r");
            message += player->GetLink();
            message += " |cFFFF80FF密|r ";
            message += receiver->GetLink();
            message += "|cFFFF80FF: " + msg + "|r";
            SendGlobalGMSysMessage(message.c_str());
            sFakePlayerMgr->m_lastWhispers[player->GetGUID()] = receiver->GetGUID();
        }
        else if(sFakePlayerMgr->m_gmFakePlayers.find(player->GetGUID()) != sFakePlayerMgr->m_gmFakePlayers.end())
        {
            ObjectGuid guid = sFakePlayerMgr->m_gmFakePlayers[player->GetGUID()];
            if(guid.IsEmpty())
            {
                if (sFakePlayerMgr->m_lastWhispers.find(receiver->GetGUID()) != sFakePlayerMgr->m_lastWhispers.end())
                    guid = sFakePlayerMgr->m_lastWhispers[receiver->GetGUID()];
            }

            if (sFakePlayerMgr->IsBotPlayer(guid))
            {
                if(Player* bot = sFakePlayerMgr->m_botPlayers[guid])
                {
                    if (bot->IsInWorld() && bot != receiver)
                    {
                        bot->Whisper(msg, Language(lang), receiver);
                        std::string message("|cffff0000[真实假人]|r");
                        message += player->GetLink();
                        message += " |cFFFF80FF使用|r ";
                        message += bot->GetLink();
                        message += " |cFFFF80FF回复|r ";
                        message += receiver->GetLink();
                        message += "|cFFFF80FF: " + msg + "|r";
                        SendGlobalGMSysMessage(message.c_str());
                        return true;
                    }
                }
            }

            player->SendSysMessage("|cffff0000[真实假人]|r未找到假人, 可能玩家没有密过假人或者假人已离线");
            return true;
        }

        return false;
    }

    void OnFreeTalentPointsChanged(Player* player, uint32 /*points*/) override
    {
        sFakePlayerMgr->ClearStorePlayerTalent(player);
    }

    void OnTalentsReset(Player* player, bool /*noCost*/) override
    {
        sFakePlayerMgr->ClearStorePlayerTalent(player);
    }

    //void OnBeforeUpdate(Player* player, uint32 p_time) override
    //{
    //    if (!sFakePlayerMgr->Enable)
    //        return;

    //    if (sFakePlayerMgr->m_botPlayers.find(player->GetGUID()) != sFakePlayerMgr->m_botPlayers.end())
    //    {
    //        if (player->IsBeingTeleported())
    //            sFakePlayerMgr->HandleTeleportAck(player);
    //    }
    //}

    void SendGlobalGMMessage(WorldPacket const* packet, WorldSession* self = nullptr, uint32 team = 0)
    {
        SessionMap const& sessions = sWorld->GetAllSessions();
        for (SessionMap::const_iterator itr = sessions.begin(); itr != sessions.end(); ++itr)
        {
            // check if session and can receive global GM Messages and its not self
            WorldSession* session = itr->second;
            if (!session || session == self || !session->HasPermission(rbac::RBAC_PERM_RECEIVE_GLOBAL_GM_TEXTMESSAGE))
                continue;

            // Player should be in world
            Player* player = session->GetPlayer();
            if (!player || !player->IsInWorld())
                continue;

            if(!sFakePlayerMgr->MessageGameMasters.empty() && !sFakePlayerMgr->MessageGameMasters.count(session->GetAccountId()))
                continue;

            // Send only to same team, if team is given
            if (!team || player->GetTeam() == team)
                session->SendPacket(packet);
        }
    }

    void SendGlobalGMSysMessage(const char *str, WorldSession* self = nullptr)
    {
        WorldPacket data;
        for (const auto& line : Tokenizer{ str, '\n' })
        {
            ChatHandler::BuildChatPacket(data, CHAT_MSG_SYSTEM, LANG_UNIVERSAL, nullptr, nullptr, line);
            SendGlobalGMMessage(&data, self);
        }
    }
};

class FakePlayerWorldSessionScript : public WorldSessionScript
{
public:
    FakePlayerWorldSessionScript() : WorldSessionScript("FakePlayerWorldSessionScript") { }

    bool OnSendPacket(WorldSession* session, WorldPacket& packet) override
    {
        if (!sFakePlayerMgr->Enable)
            return false;

        Player* player = session->GetPlayer();
        if (player && player->IsPlayerBot(true))
        {
            switch (packet.GetOpcode())
            {
                case SMSG_GROUP_INVITE:
                    return sFakePlayerMgr->HandleGroupInvite(player, packet);
                case SMSG_RESURRECT_REQUEST:
                    return sFakePlayerMgr->HandleResurrect(player, packet);
                //case SMSG_MOVE_SET_CAN_FLY:
                //{
                //    ObjectGuid guid;
                //    packet >> guid.ReadAsPacked();
                //    if (guid != player->GetGUID())
                //        return false;

                //    player->m_movementInfo.SetMovementFlags((MovementFlags)(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_CAN_FLY));
                //    return true;
                //}
                //case SMSG_MOVE_UNSET_CAN_FLY:
                //{
                //    ObjectGuid guid;
                //    packet >> guid.ReadAsPacked();
                //    if (guid != player->GetGUID())
                //        return false;

                //    player->m_movementInfo.RemoveMovementFlag(MOVEMENTFLAG_FLYING);
                //    return true;
                //}
                //case SMSG_LFG_ROLE_CHOSEN:
                //    return sFakePlayerMgr->HandleLFGRoleChosen(player, packet);
#ifndef TBC243
                case SMSG_LFG_JOIN_RESULT:
                    return sFakePlayerMgr->HandleLFGJoinResult(player, packet);
                case SMSG_LFG_ROLE_CHECK_UPDATE:
                    return sFakePlayerMgr->HandleLFGRoleCheck(player, packet);
                case SMSG_LFG_PROPOSAL_UPDATE:
                    return sFakePlayerMgr->HandleLFGProposal(player, packet);
#endif
                case SMSG_DUEL_REQUESTED:
                    return sFakePlayerMgr->HandleDuelRequested(player, packet);
                //case SMSG_CAST_FAILED:
                //{
                //    packet.rpos(0);
                //    uint8 castCount, result;
                //    uint32 spellId;
                //    packet >> castCount >> spellId >> result;
                //    if (result != SPELL_CAST_OK)
                //    {
                //        SpellInterrupted(spellId);
                //        botOutgoingPacketHandlers.AddPacket(packet);
                //    }
                //    return;
                //}
                //case SMSG_SPELL_FAILURE:
                //{
                //    packet.rpos(0);
                //    ObjectGuid casterGuid;
                //    packet >> casterGuid.ReadAsPacked();

                //    if (casterGuid != player->GetGUID())
                //        return;

                //    uint8 castCount;
                //    uint32 spellId;
                //    p >> castCount;
                //    p >> spellId;
                //    SpellInterrupted(spellId);
                //    return;
                //}
                //case SMSG_SPELL_DELAYED:
                //{
                //    packet.rpos(0);
                //    ObjectGuid casterGuid;
                //    packet >> casterGuid.ReadAsPacked();

                //    if (casterGuid != player->GetGUID())
                //        return;

                //    uint32 delaytime;
                //    p >> delaytime;
                //    if (delaytime <= 1000)
                //        IncreaseNextCheckDelay(delaytime);
                //    return;
                //}
            }

            return true;
        }

        return false;
    }

    void OnBeforeUpdate(WorldSession* session, uint32 /*p_time*/) override
    {
        if (!sFakePlayerMgr->Enable)
            return;

        Player* player = session->GetPlayer();
        if (player && player->IsPlayerBot(true))
        {
            if (player->IsBeingTeleported())
                sFakePlayerMgr->HandleTeleportAck(player);
            else if (player->IsInWorld())
                session->HandleBotPackets();
        }
    }
};

class FakePlayerAllMapScript : public AllMapScript
{
public:
    FakePlayerAllMapScript() : AllMapScript("FakePlayerAllMapScript") { }

    void OnPlayerEnterAll(Map* map, Player* player) override
    {
        if (!map->IsBattleground())
            return;

        if (sFakePlayerMgr->IsBotPlayer(player))
            sPlayerBotBGMgr->PlayerEnter((BattlegroundMap*)map, player);
    }

    void OnPlayerLeaveAll(Map* map, Player* player) override
    {
        if (!map->IsBattleground())
            return;

        if (sFakePlayerMgr->IsBotPlayer(player))
        {
            sFakePlayerMgr->SelectPlayerBotAI(player, AITYPE_FIELD, true);
            sPlayerBotBGMgr->PlayerLeave((BattlegroundMap*)map, player);
        }
    }

    void OnUpdateAll(Map* map, uint32 diff) override
    {
        if (!map->IsBattleground())
            return;

        sPlayerBotBGMgr->Update((BattlegroundMap*)map, diff);
    }
};

class FakePlayerCommandScript : public CommandScript
{
public:
    FakePlayerCommandScript() : CommandScript("FakePlayerCommandScript") { }

    std::vector<ChatCommand> GetCommands() const override
    {
        static std::vector<ChatCommand> fakePlayerGroupCommandTable =
        {
            { "all",            SEC_MODERATOR,               true,  &HandleFakePlayerGroupAllDisbandCommand, "" },
            { "",               SEC_MODERATOR,               true,  &HandleFakePlayerGroupDisbandCommand,    "" },
        };

        static std::vector<ChatCommand> fakePlayerCommandTable =
        {
            { "group",          SEC_MODERATOR,               true,  nullptr,                                 "", fakePlayerGroupCommandTable },
            { "last",           SEC_MODERATOR,               false, &HandleFakeLastPlayerCommand,            ""                              },
            { "",               SEC_MODERATOR,               false, &HandleFakePlayerCommand,                ""                              },
        };

        static std::vector<ChatCommand> commandTable =
        {
            { "fakeplayer",     SEC_MODERATOR,               true,  nullptr,                                 "", fakePlayerCommandTable      }
        };
        return commandTable;
    }

    static bool HandleFakePlayerCommand(ChatHandler* handler, char const* args)
    {
        Player* _player = handler->GetSession()->GetPlayer();
        if(args && *args)
        {
            //ObjectGuid playerGuid = sCharacterCache->GetCharacterGuidByName(playerName);

            Player* target;
            ObjectGuid targetGuid;
            std::string targetName;
            if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
            {
                handler->SendSysMessage("未找到指定假人");
                return true;
            }

            if (!sFakePlayerMgr->IsBotPlayer(targetGuid))
            {
                handler->SendSysMessage("未找到指定假人");
                return true;
            }

            sFakePlayerMgr->m_gmFakePlayers[_player->GetGUID()] = targetGuid;
            //handler->GetSession()->SendNotification("|cffff0000[真实假人]|r你已将密语回复设置为使用假人[%s]进行回复", targetName.c_str());
            handler->GetSession()->SendAreaTriggerMessage("|cffff0000[真实假人]|r你已将密语回复设置为使用假人[%s]进行回复", targetName.c_str());
        }
        else if (sFakePlayerMgr->m_gmFakePlayers.find(_player->GetGUID()) == sFakePlayerMgr->m_gmFakePlayers.end())
            return HandleFakeLastPlayerCommand(handler, nullptr);
        else
        {
            sFakePlayerMgr->m_gmFakePlayers.erase(_player->GetGUID());
            //handler->GetSession()->SendNotification("|cffff0000[真实假人]|r你已停止使用假人密语回复");
            handler->GetSession()->SendAreaTriggerMessage("|cffff0000[真实假人]|r你已停止使用假人密语回复");
        }

        return true;
    }

    static bool HandleFakeLastPlayerCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* _player = handler->GetSession()->GetPlayer();
        sFakePlayerMgr->m_gmFakePlayers[_player->GetGUID()] = ObjectGuid::Empty;
        //handler->GetSession()->SendNotification("|cffff0000[真实假人]|r你已将密语回复设置为自动使用玩家最后一次密语的假人");
        handler->GetSession()->SendAreaTriggerMessage("|cffff0000[真实假人]|r你已将密语回复设置为自动使用玩家最后一次密语的假人");
        return true;
    }

    static bool HandleFakePlayerGroupAllDisbandCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        ObjectGuid targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
        {
            handler->SendSysMessage("未找到指定假人");
            return true;
        }

        if (!sFakePlayerMgr->IsBotPlayer(targetGuid))
        {
            handler->SendSysMessage("未找到指定假人");
            return true;
        }

        if (Group* grp = target->GetGroup())
        {
            std::list<Player*> bots;
            //Group::MemberSlotList const& members = grp->GetMemberSlots();
            //for (Group::MemberSlotList::const_iterator itr = members.begin(); itr != members.end(); ++itr)
            //{
            //    Player* member = ObjectAccessor::FindPlayer((*itr).guid);
            //    if (!member)
            //        continue;

            //    if (sFakePlayerMgr->m_botPlayers.find(member->GetGUID()) == sFakePlayerMgr->m_botPlayers.end())
            //        continue;

            //    member->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
            //}

            for (GroupReference* itr = grp->GetFirstMember(); itr != nullptr; itr = itr->next())
            {
                Player* member = itr->GetSource();
                if (!member)
                    continue;

                if (sFakePlayerMgr->IsBotPlayer(member))
                    bots.push_back(member);
            }

            for(auto bot : bots)
                bot->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);


            handler->PSendSysMessage("|cffff0000[真实假人]|r指定假人[%s]所属团队/小队中的假人已全部退出", targetName.c_str());
        }
        else
            handler->PSendSysMessage("|cffff0000[真实假人]|r指定假人[%s]没有在队伍中", targetName.c_str());

        return true;
    }

    static bool HandleFakePlayerGroupDisbandCommand(ChatHandler* handler, char const* args)
    {
        Player* target;
        ObjectGuid targetGuid;
        std::string targetName;
        if (!handler->extractPlayerTarget((char*)args, &target, &targetGuid, &targetName))
        {
            handler->SendSysMessage("未找到指定假人");
            return true;
        }

        if (!sFakePlayerMgr->IsBotPlayer(targetGuid))
        {
            handler->SendSysMessage("未找到指定假人");
            return true;
        }

        Group* grp = target->GetGroup();
        Group* grpInvite = target->GetGroupInvite();
        if (!grp && !grpInvite)
        {
            handler->PSendSysMessage("|cffff0000[真实假人]|r指定假人[%s]没有在队伍中并且没有待处理的队伍邀请", targetName.c_str());
            return true;
        }

        if (target->InBattleground())
        {
            handler->PSendSysMessage("|cffff0000[真实假人]|r指定假人[%s]正在一个战场队伍中, 无法退出", targetName.c_str());
            return true;
        }

        if (grp)
        {
            target->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
            handler->PSendSysMessage("|cffff0000[真实假人]|r指定假人[%s]已成功退出团队/小队", targetName.c_str());
        }
        else if (grpInvite && grpInvite->GetLeaderGUID() == target->GetGUID())
        { // pending group creation being cancelled
            grpInvite->Disband();
            handler->PSendSysMessage("|cffff0000[真实假人]|r指定假人[%s]已拒绝团队/小队邀请", targetName.c_str());
        }

        return true;
    }
};

class FakePlayerWorldScript : public WorldScript
{
public:
    FakePlayerWorldScript() : WorldScript("FakePlayerWorldScript") { }

    void OnStartup() override
    {
        sFakePlayerMgr->LoadConfig(true);
        PlayerBotSetting::Initialize();
    }

    void OnConfigLoad(bool reload) override
    {
        sConfigMgr->LoadMore("Settings/mod_FakePlayer.conf");
        sFakePlayerMgr->LoadConfig(reload);
    }

    void ExecuteEvent(uint32 eventId)
    {
        switch (eventId)
        {
        case EVENT_UPDATE_ACCOUNT:
            sFakePlayerMgr->LoadAllAccountPlayer();
            sFakePlayerMgr->events.Repeat(30min);
            break;
        case EVENT_CHECK_PLAYERCOUNT:
            sFakePlayerMgr->CheckPlayerCount();
            sFakePlayerMgr->events.Repeat(randtime(Seconds(sFakePlayerMgr->LoginMinTime), Seconds(sFakePlayerMgr->LoginMaxTime)));
            break;
        case EVENT_REMOVE_ALL:
            sFakePlayerMgr->FakePlayerLogout();
            if(!sFakePlayerMgr->Enable && sFakePlayerMgr->GetFakePlayerCount())
                sFakePlayerMgr->events.Repeat(randtime(Seconds(sFakePlayerMgr->LoginMinTime), Seconds(sFakePlayerMgr->LoginMaxTime)));
            break;
        case EVENT_LFG_QUEUE_COOLDOWN:
            sFakePlayerMgr->LfgQueueCooldown();
            sFakePlayerMgr->events.Repeat(randtime(Seconds(sFakePlayerMgr->QueueLfgMinTime * 10), Seconds(sFakePlayerMgr->QueueLfgMaxTime * 10)));
            break;
        case EVENT_BATTLEGROUND_QUEUE_COOLDOWN:
            sFakePlayerMgr->BgQueueCooldown();
            sFakePlayerMgr->events.Repeat(randtime(Seconds(sFakePlayerMgr->QueueBgMinTime * 15), Seconds(sFakePlayerMgr->QueueBgMaxTime * 15)));
            break;
        case EVENT_LFG_QUEUE_CHECK:
            sFakePlayerMgr->CheckLfgQueue();
            sFakePlayerMgr->events.Repeat(randtime(Seconds(sFakePlayerMgr->QueueLfgMinTime), Seconds(sFakePlayerMgr->QueueLfgMaxTime)));
            break;
        case EVENT_BATTLEGROUND_QUEUE_CHECK:
            sFakePlayerMgr->CheckBGQueue();
            sFakePlayerMgr->events.Repeat(randtime(Seconds(sFakePlayerMgr->QueueBgMinTime), Seconds(sFakePlayerMgr->QueueBgMaxTime)));
            break;
        }
    }

    void OnUpdate(uint32 diff) override
    {
        sFakePlayerMgr->events.Update(diff);
        sFakePlayerMgr->scheduler.Update(diff);
        while (uint32 eventId = sFakePlayerMgr->events.ExecuteEvent())
            ExecuteEvent(eventId);
    }

    void OnShutdown() override
    {
        sFakePlayerMgr->RemoveAllPlayerBot();
    }
};

void AddSC_FakePlayer()
{
    new FakePlayerWorldScript();
    new FakePlayerWorldSessionScript();
    new FakePlayerAllMapScript();
    new FakePlayerCommandScript();
    new FakePlayerPlayerScript();
}
