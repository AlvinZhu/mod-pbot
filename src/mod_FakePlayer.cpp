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
#include "PlayerBotSetting.h"
#include "BotGroupAI.h"
#include "BotFieldAI.h"
#include "BotAI.h"
#include "BotDuelAI.h"
#include "BotArenaAI.h"
#include "AIWaypointsMgr.h"
#include "CharacterHandler.cpp"
#ifdef MODCROSSFACTIONPVP
#include "mod_CrossFactionPVP.h"
#endif
#ifdef MODLFG
#include "mod_lfg.h"
#endif

using namespace lfg;

//struct LfgQueuePlayerData
//{
//    uint8 Level;
//    TeamId TeamId;
//    LfgPlayerData const* PlayerData;
//
//    LfgQueuePlayerData() : Level(0), TeamId(TEAM_NEUTRAL), PlayerData(nullptr) {}
//};

FakePlayerMgr* FakePlayerMgr::instance()
{
    static FakePlayerMgr instance;
    return &instance;
}

FakePlayerMgr::FakePlayerMgr() : Enable(true)/*, LoginMessage(false)*/, AccountMax(1), LoginMinTime(10), LoginMaxTime(20), OnlineRate(1), OnlineMin(10), OnlineMax(999), MessageToGameMaster(true)
    , QueueLfgMinTime(30), QueueLfgMaxTime(60), QueueBgMinTime(30), QueueBgMaxTime(60), FieldCreature(true), FieldPVP(true)
{
    //events.RescheduleEvent(EVENT_UPDATE_ACCOUNT, 30min);
    //events.RescheduleEvent(EVENT_CHECK_PLAYERCOUNT, 1s);
}

void FakePlayerMgr::LoadConfig(bool reload)
{
    Enable = sConfigMgr->GetBoolDefault("FakePlayer.Enable", true);
    //LoginMessage = sConfigMgr->GetBoolDefault("FakePlayer.LoginMessage", false);
    AccountMax = sConfigMgr->GetIntDefault("FakePlayer.Account.Max", 1);
    LoginMinTime = sConfigMgr->GetIntDefault("FakePlayer.Login.MinTime", 10);
    LoginMaxTime = sConfigMgr->GetIntDefault("FakePlayer.Login.MaxTime", 20);
    OnlineRate = sConfigMgr->GetFloatDefault("FakePlayer.Online.Rate", 1.0f);
    OnlineMin = sConfigMgr->GetIntDefault("FakePlayer.Online.Min", 10);
    OnlineMax = sConfigMgr->GetIntDefault("FakePlayer.Online.Max", 999);
    MessageToGameMaster = sConfigMgr->GetBoolDefault("FakePlayer.Message.ToGameMaster", true);

    // ----- 机器人设置 ----- //
    QueueLfgMinTime = sConfigMgr->GetIntDefault("FakePlayer.Queue.LFG.MinTime", 30);
    QueueLfgMaxTime = sConfigMgr->GetIntDefault("FakePlayer.Queue.LFG.MaxTime", 60);
    QueueBgMinTime = sConfigMgr->GetIntDefault("FakePlayer.Queue.BG.MinTime", 30);
    QueueBgMaxTime = sConfigMgr->GetIntDefault("FakePlayer.Queue.BG.MaxTime", 60);
    FieldCreature = sConfigMgr->GetBoolDefault("FakePlayer.Field.Creature", true);
    FieldPVP = sConfigMgr->GetBoolDefault("FakePlayer.Field.PVP", true);

    if (!AccountMax)
        AccountMax = 1;

    AccountAllow.clear();
    std::istringstream issAllowAccount(sConfigMgr->GetStringDefault("FakePlayer.Account.Allow", ""));
    while (issAllowAccount.good())
    {
        uint32 accId;
        issAllowAccount >> accId;
        if (issAllowAccount.fail())
            break;
        AccountAllow.insert(accId);
    }

    AccountIgnore.clear();
    std::istringstream issIgnoreAccount(sConfigMgr->GetStringDefault("FakePlayer.Account.Ignore", ""));
    while (issIgnoreAccount.good())
    {
        uint32 accId;
        issIgnoreAccount >> accId;
        if (issIgnoreAccount.fail())
            break;
        AccountIgnore.insert(accId);
    }

    MessageGameMasters.clear();
    std::istringstream issAccount(sConfigMgr->GetStringDefault("FakePlayer.Message.GameMasterID", ""));
    while (issAccount.good())
    {
        uint32 accId;
        issAccount >> accId;
        if (issAccount.fail())
            break;
        MessageGameMasters.insert(accId);
    }

    if(Enable)
    {
        events.CancelEvent(EVENT_REMOVE_ALL);
        if (!events.GetNextEventTime(EVENT_UPDATE_ACCOUNT))
            events.RescheduleEvent(EVENT_UPDATE_ACCOUNT, 30min);
        if (!events.GetNextEventTime(EVENT_CHECK_PLAYERCOUNT))
            events.RescheduleEvent(EVENT_CHECK_PLAYERCOUNT, 1s);
        if (!events.GetNextEventTime(EVENT_LFG_QUEUE_CHECK))
            events.RescheduleEvent(EVENT_LFG_QUEUE_CHECK, 1s);
        if (!events.GetNextEventTime(EVENT_BATTLEGROUND_QUEUE_CHECK))
            events.RescheduleEvent(EVENT_BATTLEGROUND_QUEUE_CHECK, 1s);

        TC_LOG_INFO("server.loading", "Loading AI Way points...");
        sAIWPMgr->LoadAIWaypoints();
    }
    else
    {
        CleanData(true);
        events.CancelEvent(EVENT_UPDATE_ACCOUNT);
        events.CancelEvent(EVENT_CHECK_PLAYERCOUNT);
        events.CancelEvent(EVENT_LFG_QUEUE_CHECK);
        events.CancelEvent(EVENT_BATTLEGROUND_QUEUE_CHECK);
    }

    if (reload)
        LoadAllAccountPlayer();
}

uint32 FakePlayerMgr::GetFakePlayerCount() const
{
    return m_botPlayers.size();
}

uint32 FakePlayerMgr::GetPlayerCount() const
{
    //return sWorld->GetActiveSessionCount() - GetFakePlayerCount();
    // 假人的Session不归World的Session池管理, 所以活动连接就是真实玩家数
    return sWorld->GetActiveSessionCount();
}

uint32 FakePlayerMgr::GetPlayerBotAccount(ObjectGuid& playerGuid)
{
    for (auto const& account : m_allAccount)
    {
        for (auto const& guid : account.second)
        {
            if (guid == playerGuid)
                return account.first;
        }
    }

    return 0;
}

bool FakePlayerMgr::RemovePlayerBot(ObjectGuid playerGuid)
{
    auto const itr = m_botPlayers.find(playerGuid);
    if (itr != m_botPlayers.end())
    {
        Player* bot = itr->second;
        m_botPlayers.erase(itr);
        WorldSession* session = bot->GetSession();
        session->LogoutPlayer(false);
        delete session;

        return true;
    }

    return false;
}

bool FakePlayerMgr::RemoveAccountBot(uint32 accountId, bool all/* = false*/)
{
    auto& account = m_botAccount[accountId];
    for (auto itr = account.begin(); itr != account.end(); )
    {
        bool bRet = RemovePlayerBot(*itr);
        itr = account.erase(itr);
        if (bRet && !all)
            return true;
        //auto const pItr = m_botPlayers.find(*itr);
        //if (pItr != m_botPlayers.end())
        //{
        //    Player* bot = pItr->second;
        //    WorldSession* session = bot->GetSession();
        //    session->LogoutPlayer(false);
        //    delete session;
        //    m_botPlayers.erase(pItr);
        //    itr = account.erase(itr);
        //}
        //else
        //    ++itr;
    }

    return false;
}

void FakePlayerMgr::HandlePlayerLoginOpcode(ObjectGuid& playerGuid)
{
    // 清除登录帐号被使用的假人
    if (uint32 accountId = GetPlayerBotAccount(playerGuid))
        RemoveAccountBot(accountId, true);
}

bool FakePlayerMgr::FakePlayerLogin()
{
    SessionMap const& sessions = sWorld->GetAllSessions();
    for (auto const& account : m_allAccount)
    {
        uint32 accountId = account.first;

        if ((!AccountAllow.empty() && !AccountAllow.count(accountId)) || AccountIgnore.count(accountId))
            continue;

        if (sessions.find(accountId) != sessions.end())
            continue;

        if (m_botAccount[accountId].size() < AccountMax)
        {
            for (auto const& guid : account.second)
            {
                //ObjectGuid playerGuid(HighGuid::Player, lowGuid);
                // has bot already been added?
                Player* bot = ObjectAccessor::FindPlayer(guid);
                if (bot && bot->IsInWorld())
                    continue;

                // 已经存在的假人没必要再重新登录一遍
                auto const itr = m_botPlayers.find(guid);
                if (itr != m_botPlayers.end())
                    continue;

                // 比如被GM召唤, 如果跨地图会召唤失败导致假人掉线
                // 所以如果已存在于假人列表中, 清理一次
                //RemovePlayerBot((ObjectGuid &)guid);
                LoginQueryHolder *holder = new LoginQueryHolder(accountId, guid);
                if (!holder->Initialize())
                {
                    delete holder;                                      // delete all unprocessed queries
                    continue;
                }

                auto _charLoginCallback = CharacterDatabase.DelayQueryHolder(holder);
                _charLoginCallback.get();

                WorldSession *botSession = new WorldSession(accountId, "", nullptr, SEC_PLAYER, 2, 0, LOCALE_zhCN, 0, false);

                botSession->HandlePlayerLogin(holder); // will delete lqh

                bot = botSession->GetPlayer();
                if (!bot)
                {
                    TC_LOG_ERROR("modules", "玩家 (GUID: %u) 帐户 (AccountId: %u) 假人上线失败", guid, accountId);
                    botSession->LogoutPlayer(false);
                    delete botSession;
                    // 如果加载玩家失败, 则已在HandlePlayerLogin中清理过holder
                    //delete holder;
                    continue;
                }

                //m_botAccount[accountId].push_back(guid);
                botSession->SetSessionBot(true);
                bot->SetPlayerBot(true);
                //bot->SetBotAI(new PlayerBotAI(bot));
                SwitchPlayerBotAI(bot);
                //bot->SetCanFly(true);
                //bot->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
                //bot->SetControlled(true, UNIT_STATE_ROOT);
                //uint32 moveFlag = MOVEMENTFLAG_CAN_FLY;
                //// 拥有飞行光环, 移动状态设置为飞行中
                //if (bot->HasAuraType(SPELL_AURA_FLY) ||
                //    bot->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) ||
                //    bot->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED))
                //    moveFlag |= MOVEMENTFLAG_FLYING;
                //bot->m_movementInfo.AddMovementFlag(moveFlag);
                m_botAccount[accountId].emplace_back(guid);
                m_botPlayers[guid] = bot;
                return true;
            }
        }
    }

    return false;
}

bool FakePlayerMgr::FakePlayerLogout(bool check/* = false*/)
{
    // 优先移除不符合条件的假人
    for (auto& account : m_botAccount)
    {
        uint32 accountId = account.first;

        if ((!AccountAllow.empty() && !AccountAllow.count(accountId)) || AccountIgnore.count(accountId))
            if (RemoveAccountBot(accountId))
                return true;

        if (account.second.size() > AccountMax)
        {
            for (auto itr = account.second.begin(); itr != account.second.end();)
            {
                bool bRet = RemovePlayerBot(*itr);
                itr = account.second.erase(itr);

                if (bRet)
                    return true;

                if (account.second.size() == AccountMax)
                    break;
            }
        }
    }

    if(!check)
    {
        // 如果全都符合条件, 直接移除一个列表中的假人
        for (auto& account : m_botAccount)
        {
            uint32 accountId = account.first;

            if (RemoveAccountBot(accountId))
                return true;
        }
    }

    return false;
}

void FakePlayerMgr::CheckPlayerCount()
{
    if (!Enable)
        return;

    uint32 playerCount = GetPlayerCount();
    uint32 fakeCount = GetFakePlayerCount();
    uint32 maxBot = uint32(double(playerCount) * OnlineRate);
    maxBot = std::max(maxBot, OnlineMin);
    maxBot = std::min(maxBot, OnlineMax);
    if (fakeCount < maxBot)
        if (FakePlayerLogin())
            return;

    // 尝试移除一个不符合条件的假人, 移除成功则等待下一次检查
    if (FakePlayerLogout(true))
        return;

    if (fakeCount > maxBot)
        FakePlayerLogout();
}

void FakePlayerMgr::CleanData(bool all/* = false*/)
{
    for (auto& account : m_allAccount)
        account.second.clear();
    m_allAccount.clear();

    if(all)
        events.RescheduleEvent(EVENT_REMOVE_ALL, 1);
}

void FakePlayerMgr::RemoveAllPlayerBot()
{
    for (auto& account : m_botAccount)
        RemoveAccountBot(account.first);

    m_botAccount.clear();
}

bool FakePlayerMgr::IsBotPlayer(ObjectGuid& playerGuid)
{
    if (m_botPlayers.find(playerGuid) != m_botPlayers.end())
        return true;

    return false;
}

bool FakePlayerMgr::IsBotPlayer(Player* player)
{
    if (!player)
        return false;

    return IsBotPlayer(player->GetGUID());
}

uint8 FakePlayerMgr::FindTalentType(Player* player)
{
    if (!player)
        return 0;

    return PlayerBotSetting::FindPlayerTalentType(player);
}

bool FakePlayerMgr::IsTankPlayer(Player* player)
{
    if (!player)
        return false;

    if (player->GetLevel() < 10)
        return false;

    switch (player->GetClass())
    {
        case CLASS_WARRIOR:
            return FindTalentType(player) == SPEC_WARRIOR_PROTECTION;
        case CLASS_PALADIN:
        case CLASS_DRUID:
            return FindTalentType(player) == 1;
        case CLASS_DEATH_KNIGHT:
            return FindTalentType(player) != SPEC_DEATH_KNIGHT_UNHOLY;
    }

    return false;
}

bool FakePlayerMgr::IsHealPlayer(Player* player)
{
    if (!player)
        return false;

    if (player->GetLevel() < 10)
        return false;

    switch (player->GetClass())
    {
        case CLASS_PALADIN:
            return FindTalentType(player) == SPEC_PALADIN_HOLY;
        case CLASS_PRIEST:
            return FindTalentType(player) != SPEC_PRIEST_SHADOW;
        case CLASS_SHAMAN:
        case CLASS_DRUID:
            return FindTalentType(player) == 2;
    }

    return false;
}

bool FakePlayerMgr::IsDpsPlayer(Player* player)
{
    if (!player)
        return false;

    switch (player->GetClass())
    {
        case CLASS_WARRIOR:
            return FindTalentType(player) != SPEC_WARRIOR_PROTECTION;
        case CLASS_PALADIN:
            return FindTalentType(player) == SPEC_PALADIN_RETRIBUTION;
        case CLASS_PRIEST:
            return FindTalentType(player) == SPEC_PRIEST_SHADOW;
        case CLASS_SHAMAN:
            return FindTalentType(player) != SPEC_SHAMAN_RESTORATION;
        case CLASS_DRUID:
            return FindTalentType(player) != SPEC_DRUID_RESTORATION;
    }

    return true;
}

uint8 FakePlayerMgr::CurrentTalentPoints(Player* player)
{
    if (!player)
        return 0;

    uint8 curTalent = FindTalentType(player);
    auto itr = m_playerTalents.find(player->GetGUID());
    if (itr != m_playerTalents.end())
        return itr->second.PageCount[curTalent];

    return 0;
}

void FakePlayerMgr::ClearStorePlayerTalent(Player* player)
{
    if (!player)
        return;

    // PlayerTalentContainer::iterator
    auto itr = m_playerTalents.find(player->GetGUID());
    if (itr != m_playerTalents.end())
        m_playerTalents.erase(itr);
}

void FakePlayerMgr::PlayerBotRoll(Group* group, Player* player, Roll const& roll)
{
    if (!player)
        return;
    if (PlayerBotSetting::IsBetterEquip(player, sObjectMgr->GetItemTemplate(roll.itemid), roll.itemRandomPropId))
        group->CountRollVote(player->GetGUID(), roll.itemGUID, ROLL_NEED);
    else
        group->CountRollVote(player->GetGUID(), roll.itemGUID, ROLL_PASS);
}

bool FakePlayerMgr::GiveAtGroupPos(Group* group, ObjectGuid& guid, uint32& index, uint32& count)
{
    Group::MemberSlotList const& members = group->GetMemberSlots();
    index = 0;
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        ++index;
        if (citr->guid == guid)
        {
            count = members.size();
            return true;
        }
    }
    return false;
}

bool FakePlayerMgr::GroupExistRealPlayer(Group* group)
{
    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player)
            continue;
        if (!player->IsPlayerBot(true))
            return true;
    }
    return false;
}

bool FakePlayerMgr::GroupExistPlayerBot(Group* group)
{
    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player)
            continue;
        if (player->IsPlayerBot(true))
            return true;
    }
    return false;
}

bool FakePlayerMgr::AllGroupNotCombat(Group* group)
{
    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator itr = members.begin(); itr != members.end(); ++itr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(itr->guid);
        if (!player)
            continue;
        if (!player->IsAlive())
            continue;
        if (player->IsInCombat())
            return false;
    }

    return true;
}

bool FakePlayerMgr::AllGroupIsIDLE(Group* group)
{
    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player)
            continue;
        BotGroupAI* pAI = dynamic_cast<BotGroupAI*>(player->GetAI());
        if (!pAI)
            continue;
        if (!pAI->IsIDLEBot())
            return false;
    }

    return true;
}

void FakePlayerMgr::AllGroupBotGiveXP(Group* group, uint32 XP)
{
    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player || !player->IsPlayerBot())
            continue;
        BotGroupAI* pAI = dynamic_cast<BotGroupAI*>(player->GetAI());
        if (!pAI)
            continue;
        pAI->DelayGiveXP(XP);
    }
}

Unit* FakePlayerMgr::GetGroupTankTarget(Group* group)
{
    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player || !IsTankPlayer(player))
            continue;
        return player->GetSelectedUnit();
    }

    return nullptr;
}

std::vector<ObjectGuid> FakePlayerMgr::GetGroupMemberFromNeedRevivePlayer(Group* group, uint32 forMap)
{
    Group::MemberSlotList const& members = group->GetMemberSlots();
    std::vector<ObjectGuid> needRevivePlayers;
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player || player->IsAlive() || player->IsPlayerBot() || player->GetMapId() != forMap)
            continue;
        needRevivePlayers.push_back(citr->guid);
    }
    return needRevivePlayers;
}

void FakePlayerMgr::ResetRaidDungeon(Group* group)
{
    if (group->isBFGroup() || group->isBGGroup())
        return;
    Player* pLeader = nullptr;
    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::MemberSlot const& slot : members)
    {
        Player* groupPlayer = ObjectAccessor::FindPlayer(slot.guid);
        if (!groupPlayer)
            continue;
        if (!groupPlayer->IsInWorld() || groupPlayer->GetMap()->IsDungeon())
            return;
        if (groupPlayer->GetGUID() == group->GetLeaderGUID())
            pLeader = groupPlayer;
    }
    std::list<uint32> mapIDs;
    for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
    {
        auto boundInstance = group->GetBoundInstances(Difficulty(i));
        if (boundInstance.empty())
            continue;
        for (auto itr = boundInstance.begin(); itr != boundInstance.end(); ++itr)
        {
            InstanceSave* instanceSave = itr->second.save;
            const MapEntry* mapEntry = sMapStore.LookupEntry(itr->first);
            if (!mapEntry)
                continue;
            mapIDs.push_back(mapEntry->MapID);
        }
    }
    for (uint32 mapID : mapIDs)
    {
        for (uint8 diff = 0; diff < MAX_RAID_DIFFICULTY; ++diff)
        {
            if (GetMapDifficultyData(mapID, Difficulty(diff)))
                sInstanceSaveMgr->ForceGlobalReset(mapID, Difficulty(diff));
        }
        if (pLeader)
            pLeader->SendResetInstanceSuccess(mapID);
    }
    //ResetInstances(INSTANCE_RESET_ALL, false, pLeader);
}

void FakePlayerMgr::ClearAllGroupForceFleeState(Group* group)
{
    if (group->isBGGroup())
        return;

    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player || !player->IsPlayerBot())
            continue;
        UnitAI* pUnitAi = player->GetAI();
        if (!pUnitAi)
            continue;
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(pUnitAi))
        {
            pGroupAI->SetForceFleeState(false);
            pGroupAI->SetSeduceTarget(ObjectGuid::Empty);
        }
    }
}

void FakePlayerMgr::ProcessGroupBotCommand(Group* group, Player* srcPlayer, std::string& cmd)
{
    if (!srcPlayer || !group->IsLeader(srcPlayer->GetGUID()))
        return;
    std::string groupProcess = cmd;
    std::string groupParam;
    if (groupProcess.empty())
        return;

    Group::MemberSlotList const& members = group->GetMemberSlots();
    if (srcPlayer && !srcPlayer->InBattleground() && groupProcess == "seduce")
    {
        Creature* pSeduceTarget = nullptr;
        if (srcPlayer->GetTarget() != ObjectGuid::Empty)
        {
            Unit* pTarget = srcPlayer->GetSelectedUnit();
            if (pTarget && srcPlayer->GetDistance(pTarget->GetPosition()) < BOTAI_FIELDTELEPORT_DISTANCE &&
                srcPlayer->GetMap() == pTarget->GetMap() && srcPlayer->IsValidAttackTarget(pTarget))
                pSeduceTarget = pTarget->ToCreature();
        }
        if (!pSeduceTarget)
            pSeduceTarget = SearchSeduceCreature(srcPlayer);
        if (!pSeduceTarget)
            return;
        BotGroupAI* pSeduceGroupAI = SearchExecuteSeduceBotAI(group);
        if (!pSeduceGroupAI)
            return;
        for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
        {
            Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
            if (!player || !player->IsPlayerBot() || srcPlayer->GetMap() != player->GetMap())
                continue;
            UnitAI* pUnitAi = player->GetAI();
            if (!pUnitAi)
                continue;
            if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(pUnitAi))
            {
                if (pGroupAI == pSeduceGroupAI)
                {
                    pGroupAI->SetForceFleeState(false);
                    pGroupAI->SetSeduceTarget(pSeduceTarget->GetGUID());
                }
                else
                {
                    pGroupAI->SetForceFleeState(true);
                    pGroupAI->SetSeduceTarget(ObjectGuid::Empty);
                }
            }
        }
        return;
    }
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player || !player->IsPlayerBot(true))
            continue;
        UnitAI* pUnitAi = player->GetAI();
        if (BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(pUnitAi))
            pGroupAI->ProcessBotCommand(srcPlayer, cmd);
        else// if (player->IsPlayerBot() && srcPlayer->InArena())
        {
            if (BotBGAI* pBGAI = dynamic_cast<BotBGAI*>(pUnitAi))
                pBGAI->ProcessBotCommand(srcPlayer, cmd);
        }
    }

    BOTAI_WORKTYPE botaiType = AIWT_ALL;
    if (groupProcess[0] == '@')
    {
        int32 firstEndIndex = groupProcess.find(' ');
        if (firstEndIndex <= 1)
            return;
        std::string target = groupProcess.substr(1, firstEndIndex - 1);
        std::string realCmd = groupProcess.substr(firstEndIndex + 1);
        if (realCmd.empty())
            return;
        if (target == "tank")
        {
            botaiType = AIWT_TANK;
        }
        else if (target == "melee")
        {
            botaiType = AIWT_MELEE;
        }
        else if (target == "ranged")
        {
            botaiType = AIWT_RANGE;
        }
        else if (target == "heal")
        {
            botaiType = AIWT_HEAL;
        }

        int32 secondEndIndex = realCmd.find(' ');
        if (secondEndIndex <= 0)
            groupProcess = realCmd;
        else
        {
            groupProcess = realCmd.substr(0, secondEndIndex);
            groupParam = realCmd.substr(secondEndIndex + 1);
        }
    }
    else
    {
        int32 firstEndIndex = groupProcess.find(' ');
        if (firstEndIndex < 0)
            return;
        std::string realCmd = groupProcess.substr(0, firstEndIndex);
        groupParam = groupProcess.substr(firstEndIndex + 1);
        groupProcess = realCmd;
    }
    if (groupProcess == "pulls")
        BotUtility::ProcessGroupTankPullTargets(srcPlayer);
    else if (groupProcess == "formation")
    {
        if (groupParam == "combat")
            BotUtility::ProcessGroupCombatMovement(srcPlayer, botaiType);
        else if (groupParam == "ring")
            BotUtility::ProcessGroupRingMovement(srcPlayer, botaiType);
        else if (groupParam == "random")
        {
            for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
            {
                Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
                if (!player || !player->IsPlayerBot())
                    continue;
                BotGroupAI* pGroupAI = dynamic_cast<BotGroupAI*>(player->GetAI());
                if (!pGroupAI)
                    continue;
                if (pGroupAI->IsMeleeBotAI())
                {
                    if (botaiType == AIWT_ALL || botaiType == AIWT_TANK || botaiType == AIWT_MELEE)
                        pGroupAI->RndCruxMovement();
                }
                else
                {
                    if (botaiType == AIWT_ALL || botaiType == AIWT_RANGE || botaiType == AIWT_HEAL)
                        pGroupAI->RndCruxMovement();
                }
            }
        }
    }
}

void FakePlayerMgr::OnLeaderChangePhase(Group* group, Player* changeTarget, uint32 newPhase)
{
    if (group->isBGGroup() || group->isBFGroup())
        return;
    if (!changeTarget || !changeTarget->IsInWorld() || !group->IsLeader(changeTarget->GetGUID()) || changeTarget->IsPlayerBot())
        return;

    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
        {
            if (player == changeTarget || !player->IsPlayerBot() || player->GetPhaseMask() == newPhase)
                continue;
            player->SetPhaseMask(newPhase, false);
            player->GetSession()->SendSetPhaseShift(newPhase);
        }
    }
}

Creature* FakePlayerMgr::SearchSeduceCreature(Player* centerPlayer)
{
    std::map<uint32, Creature*> creatures;
    std::list<Creature*> nearCreature;
    Trinity::AllWorldObjectsInRange checker(centerPlayer, BOTAI_FIELDTELEPORT_DISTANCE * 1.2f);
    Trinity::CreatureListSearcher<Trinity::AllWorldObjectsInRange> searcher(centerPlayer, nearCreature, checker);
    //centerPlayer->VisitNearbyGridObject(BOTAI_FIELDTELEPORT_DISTANCE * 1.2f, searcher);
    Cell::VisitGridObjects(centerPlayer, searcher, BOTAI_FIELDTELEPORT_DISTANCE * 1.2f);
    for (Creature* pCreature : nearCreature)
    {
        if (!pCreature->IsAlive() || !pCreature->IsVisible() || pCreature->IsPet() || pCreature->IsTotem() || pCreature->GetLevel() <= 1)
            continue;
        if (pCreature->IsInEvadeMode() || pCreature->IsInCombat() || pCreature->GetTarget() != ObjectGuid::Empty)
            continue;
        if (!centerPlayer->IsValidAttackTarget(pCreature))
            continue;
        uint32 dist = BotAIMovement::GetTargetFindpathPointCount(centerPlayer, pCreature);
        creatures[dist] = pCreature;
    }
    uint32 minDist = 99999;
    Creature* selectCreature = nullptr;
    for (std::map<uint32, Creature*>::iterator itCreature = creatures.begin();
        itCreature != creatures.end(); itCreature++)
    {
        Creature* creature = itCreature->second;
        uint32 dist = itCreature->first;
        if (!selectCreature || dist < minDist)
        {
            minDist = dist;
            selectCreature = creature;
        }
    }
    return selectCreature;
}

BotGroupAI* FakePlayerMgr::SearchExecuteSeduceBotAI(Group* group)
{
    uint32 maxPriority = 0;
    std::vector<BotGroupAI*> seduceAIs;

    Group::MemberSlotList const& members = group->GetMemberSlots();
    for (Group::member_citerator citr = members.begin(); citr != members.end(); ++citr)
    {
        Player* player = ObjectAccessor::FindConnectedPlayer(citr->guid);
        if (!player)
            continue;
        BotGroupAI* pAI = dynamic_cast<BotGroupAI*>(player->GetAI());
        if (!pAI || !pAI->CanExecuteSeduce())
            continue;
        uint32 seducePriority = pAI->GetSeducePriority();
        if (seducePriority == 0)
            continue;
        if (seducePriority > maxPriority)
        {
            seduceAIs.clear();
            maxPriority = seducePriority;
            seduceAIs.push_back(pAI);
        }
        else if (seducePriority == maxPriority)
            seduceAIs.push_back(pAI);
    }
    if (seduceAIs.empty())
        return nullptr;
    return seduceAIs[urand(0, seduceAIs.size() - 1)];
}

void FakePlayerMgr::LfgQueueCooldown()
{
    if (m_QueueLfgCooldownStore.empty())
        return;

    m_QueueLfgCooldownStore.erase(m_QueueLfgCooldownStore.begin());
}

void FakePlayerMgr::CheckLfgQueue()
{
    // 排本逻辑, 在完善AI以及解决飞天问题前, 不启用排本功能
    return;
    auto PlayersStore = sLFGMgr->GetPlayersStore();
    if (PlayersStore.empty())
        return;

    bool crossTeam = sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP)
#ifdef MODLFG
        || sModLFGMgr->CrossFaction
#endif
        ;

    //std::list<LfgQueuePlayerData> processList;
    std::unordered_map<uint16, LfgPlayerData const*> processMap;
    std::unordered_map<ObjectGuid, uint8> botGuids;
    std::set<uint8> levelSet;
    //uint8 processLevel = 0;
    //TeamId processTeam = TEAM_NEUTRAL;
    //LfgPlayerData const* processData = nullptr;
    //GuidSet botGuids;
    for (LfgPlayerDataContainer::iterator itLfgPlayer = PlayersStore.begin(); itLfgPlayer != PlayersStore.end(); ++itLfgPlayer)
    {
        LfgPlayerData const& lfgPlayer = itLfgPlayer->second;
        if (lfgPlayer.GetState() != LFG_STATE_QUEUED)
            continue;

        ObjectGuid const& playerGUID = itLfgPlayer->first;
        Player* player = ObjectAccessor::FindPlayer(playerGUID);
        if (!player || !player->IsInWorld())
            continue;

        if (IsBotPlayer(player))
        {
            //botGuids.insert(playerGUID);
            botGuids.emplace(playerGUID, player->GetLevel());
            continue;
        }

        //if (processData)
        //    continue;

        //processData = &lfgPlayer;
        //processLevel = player->getLevel();
        TeamId team = crossTeam ? TEAM_NEUTRAL : player->GetTeamId();
        uint16 key = player->GetLevel() << 8 | team;

        if (processMap.find(key) != processMap.end())
            continue;

        levelSet.insert(player->GetLevel());
        //processList.emplace_back(player->getLevel(), player->GetTeamId(), &lfgPlayer);
        processMap.emplace(key, &lfgPlayer);
    }

    for (auto& bot : botGuids)
        if(!levelSet.count(bot.second))
            sLFGMgr->LeaveLfg(bot.first);
    botGuids.clear();

    if (processMap.empty())
        return;

    for (auto& item : processMap)
        BotJoinLfgQueue(item.first >> 8, item.first & 0xFF, item.second);
}

bool FakePlayerMgr::BotJoinLfgQueue(uint8 level, uint8 teamId, LfgPlayerData const* playerData)
{
    if (!playerData)
        return false;

    auto PlayersStore = sLFGMgr->GetPlayersStore();
    if (PlayersStore.empty())
        return false;

    int32 needTank = LFG_TANKS_NEEDED;
    int32 needDPS = LFG_DPS_NEEDED;
    int32 needHeal = LFG_HEALERS_NEEDED;
    uint8 proRoles = playerData->GetRoles();
    if (proRoles & lfg::LfgRoles::PLAYER_ROLE_TANK)
        --needTank;
    else if (proRoles & lfg::LfgRoles::PLAYER_ROLE_DAMAGE)
        --needDPS;
    else if (proRoles & lfg::LfgRoles::PLAYER_ROLE_HEALER)
        --needHeal;

    for (lfg::LfgPlayerDataContainer::iterator itLfgPlayer = PlayersStore.begin(); itLfgPlayer != PlayersStore.end(); ++itLfgPlayer)
    {
        const ObjectGuid& playerGUID = itLfgPlayer->first;
        const lfg::LfgPlayerData& lfgPlayer = itLfgPlayer->second;
        if (lfgPlayer.GetState() != lfg::LFG_STATE_QUEUED || playerData == &lfgPlayer)
            continue;

        Player* player = ObjectAccessor::FindPlayer(playerGUID);
        if (!player || !IsBotPlayer(player) || !player->IsInWorld() || player->GetMap()->IsDungeon() || player->GetLevel() != level)
            continue;

        uint8 role = lfgPlayer.GetRoles();
        if (role == lfg::LfgRoles::PLAYER_ROLE_TANK)
            --needTank;
        else if (role == lfg::LfgRoles::PLAYER_ROLE_DAMAGE)
            --needDPS;
        else if (role == lfg::LfgRoles::PLAYER_ROLE_HEALER)
            ----needHeal;
    }

    if (needTank <= 0 && needDPS <= 0 && needHeal <= 0)
        return true;

    LfgRoles needRole = PLAYER_ROLE_NONE;
    if (needTank > 0)
        needRole = PLAYER_ROLE_TANK;
    else if (needDPS > 0)
        needRole = PLAYER_ROLE_DAMAGE;
    else if (needHeal > 0)
        needRole = PLAYER_ROLE_HEALER;

    for (auto& botItem : m_botPlayers)
    {
        if (m_QueueLfgCooldownStore.count(botItem.first))
            continue;

        Player* bot = botItem.second;

        if (bot->InBattleground() || bot->InArena() || bot->GetMap()->IsDungeon() || bot->GetGroup())
            continue;

        if (bot->isUsingLfg() || bot->InBattlegroundQueue())
            continue;

        if (bot->GetLevel() != level)
            continue;

        if (teamId != TEAM_NEUTRAL && bot->GetTeamId() != teamId)
            continue;

        if (bot->HasAura(LFG_SPELL_DUNGEON_DESERTER))
            //continue;
            bot->RemoveAurasDueToSpell(LFG_SPELL_DUNGEON_DESERTER);
        if (bot->HasAura(LFG_SPELL_DUNGEON_COOLDOWN))
            //continue;
            bot->RemoveAurasDueToSpell(LFG_SPELL_DUNGEON_COOLDOWN);
        //if (bot->HasAura(26013))
        //    //continue;
        //    bot->RemoveAurasDueToSpell(26013);
        if (bot->HasAura(9454))
            //continue;
            bot->RemoveAurasDueToSpell(9454);

        //if(sLFGMgr->GetState(botItem.first) == LFG_STATE_QUEUED)
        //    continue;

        //PlayerBotAI* ai = GetPlayerBotAI(bot);
        //if(!ai || (ai->GetTypeAI() != AITYPE_NULL && ai->GetTypeAI() != AITYPE_FIELD))
        //    continue;

        switch (needRole)
        {
            case PLAYER_ROLE_TANK:
                if (!IsTankPlayer(bot))
                    continue;
                break;
            case PLAYER_ROLE_HEALER:
                if (!IsHealPlayer(bot))
                    continue;
                break;
            case PLAYER_ROLE_DAMAGE:
                if (!IsDpsPlayer(bot))
                    continue;
                break;
        }

        uint32 count = playerData->GetSelectedDungeons().size();
        std::vector<uint32> dungeons;
        for (auto& item : playerData->GetSelectedDungeons())
            dungeons.push_back(item);

        WorldPacket data;
        data << needRole;
        data << uint16(0);
        data << uint8(count > 3 ? 3 : count);
        if (count >= 1)
            data << dungeons[0];
        if (count >= 2)
            data << dungeons[1];
        if (count >= 3)
            data << dungeons[2];
        data << uint32(0);
        data << "";
        bot->GetSession()->HandleLfgJoinOpcode(data);

        return true;
    }

    return false;
}

uint8 FakePlayerMgr::GenerateLfgRoles(Player* player)
{
    int spec = FindTalentType(player);
    switch (player->GetClass())
    {
    case CLASS_WARRIOR:
        if (spec == SPEC_WARRIOR_PROTECTION)
            return PLAYER_ROLE_TANK;
        break;
    case CLASS_PALADIN:
        if (spec == SPEC_PALADIN_PROTECTION)
            return PLAYER_ROLE_TANK;
        else if (spec == SPEC_PALADIN_HOLY)
            return PLAYER_ROLE_HEALER;
        break;
    //case CLASS_HUNTER:
    //case CLASS_ROGUE:
    case CLASS_PRIEST:
        if (spec != SPEC_PRIEST_SHADOW)
            return PLAYER_ROLE_HEALER;
        break;
    case CLASS_DEATH_KNIGHT:
        if (spec == SPEC_DEATH_KNIGHT_BLOOD)
            return PLAYER_ROLE_TANK;
        else if (spec == SPEC_DEATH_KNIGHT_FROST)
            return PLAYER_ROLE_TANK | PLAYER_ROLE_DAMAGE;
        break;
    case CLASS_SHAMAN:
        if (spec == SPEC_SHAMAN_RESTORATION)
            return PLAYER_ROLE_HEALER;
        break;
    //case CLASS_MAGE:
    //case CLASS_WARLOCK:
    case CLASS_DRUID:
        if (spec == SPEC_DRUID_RESTORATION)
            return PLAYER_ROLE_HEALER;
        else if (spec == SPEC_DRUID_FERAL)
            return PLAYER_ROLE_TANK | PLAYER_ROLE_DAMAGE;
        else
            return PLAYER_ROLE_DAMAGE;
    }

    return PLAYER_ROLE_DAMAGE;
}

void FakePlayerMgr::BgQueueCooldown()
{
    if (m_QueueBgCooldownStore.empty())
        return;

    m_QueueBgCooldownStore.erase(m_QueueBgCooldownStore.begin());
}

void FakePlayerMgr::CheckBGQueue()
{
}

void FakePlayerMgr::TaskSafe(Milliseconds time, uint32 eventId, Player* player, std::function<void(Player*)> const& callback)
{
    ObjectGuid playerGuid = player->GetGUID();
    scheduler.Schedule(time, playerGuid, eventId, [playerGuid, callback](TaskContext task)
    {
        if (Player* bot = ObjectAccessor::FindPlayer(playerGuid))
            if (bot->IsPlayerBot(true))
                callback(bot);
    });
}

void FakePlayerMgr::TaskSafe(Milliseconds min, Milliseconds max, uint32 eventId, Player* player, std::function<void(Player*)> const& callback)
{
    TaskSafe(Milliseconds(urand(uint32(min.count()), uint32(max.count()))), eventId, player, callback);
}

void FakePlayerMgr::TaskOneSafe(Milliseconds min, Milliseconds max, uint32 eventId, Player* player, std::function<void(Player*)> const& callback)
{
    if (scheduler.HasGuidAndEventID(player->GetGUID(), eventId))
        return;

    TaskSafe(min, max, eventId, player, callback);
}

void FakePlayerMgr::TaskSafe(Milliseconds time, Player* player, std::function<void(Player*)> const& callback)
{
    ObjectGuid guid = player->GetGUID();
    scheduler.Schedule(time, [guid, callback](TaskContext task)
    {
        if (Player* bot = ObjectAccessor::FindPlayer(guid))
            if (bot->IsPlayerBot(true))
                callback(bot);
    });
}

void FakePlayerMgr::TaskSafe(Milliseconds min, Milliseconds max, Player* player, std::function<void(Player*)> const& callback)
{
    TaskSafe(Milliseconds(urand(uint32(min.count()), uint32(max.count()))), player, callback);
}

PlayerBotAI* FakePlayerMgr::GetPlayerBotAI(Player* bot)
{
    if (bot->IsAIEnabled())
        if (UnitAI* ai = bot->GetAI())
            return dynamic_cast<PlayerBotAI*>(ai);

    return nullptr;
}

void FakePlayerMgr::SelectPlayerBotAI(Player* bot, PlayerBotAIType aiType, bool force)
{
    // 选择AI的逻辑, 在继续完善前, 暂时只当一个安静的假人
   // return;
    if (!bot)
        return;

    PlayerBotAI* botAI = GetPlayerBotAI(bot);
    if (botAI && botAI->GetTypeAI() == aiType)
        return;

    if (!force && bot->IsInCombat())
        return;

    if (force && bot->IsInCombat())
        bot->ClearInCombat();

    bot->SetSelection(ObjectGuid::Empty);

    switch (aiType)
    {
        case AITYPE_FIELD:
            if (botAI = BotFieldAI::CreateBotFieldAIByPlayerClass(bot))
            {
                botAI->Reset();
                bot->SetBotAI(botAI);
            }
            break;
        case AITYPE_GROUP:
            if (botAI = BotGroupAI::CreateBotGroupAIByPlayerClass(bot))
            {
                botAI->Reset();
                bot->SetBotAI(botAI);
            }
            break;
        case AITYPE_DUEL:
            if (botAI = BotDuelAI::CreateBotDuelAIByPlayerClass(bot))
            {
                botAI->Reset();
                bot->SetBotAI(botAI);
            }
            break;
    case AITYPE_ARENA:
#ifdef CONVERT_ARENAAI_TOBG
        if (botAI = BotArenaAI::CreateBotArenaAIByPlayerClass(bot))
        {
            botAI->Reset();
            bot->SetBotAI(botAI);
        }
        break;
#endif
    case AITYPE_BG:
        if (botAI = BotBGAI::CreateBotBGAIByPlayerClass(bot))
        {
            botAI->Reset();
            bot->SetBotAI(botAI);
        }
        break;
        //case PlayerBotAIType::PBAIT_DUNGEON:
        //    if (pAI)
        //    {
        //        //if (dynamic_cast<BotFieldAI*>(pAI) != nullptr)
        //        //{
        //        //	player->IsAIEnabled = true;
        //        //	return;
        //        //}
        //        //PlayerBotMgr::m_DelayDestroyAIs[getMSTime()].push_back(pAI);
        //        player->SetAI(nullptr);
        //    }
        //    break;
    }
}

void FakePlayerMgr::SwitchPlayerBotAI(Player* bot, bool force)
{
    if (!bot)
        return;

    PlayerBotAIType typeAI = AITYPE_NULL;
    if (Battleground* bg = bot->GetBattleground())
    {
        if (bg->isArena())
            typeAI = AITYPE_ARENA;
        else
            typeAI = AITYPE_BG;
    }
    else if (bot->duel && bot->duel->State != DUEL_STATE_COMPLETED && bot->duel->Opponent)
        typeAI = AITYPE_DUEL;
    else if (bot->GetGroup())
        typeAI = AITYPE_GROUP;
    else
        typeAI = AITYPE_FIELD;

    SelectPlayerBotAI(bot, typeAI, force);
}

void FakePlayerMgr::LoadAllAccountPlayer()
{
    if (!Enable)
        return;

    uint32 oldMSTime = getMSTime();

    CleanData();

    //                                                    0       1
    QueryResult result = CharacterDatabase.Query("Select `guid`, `account` From `characters` Where `deleteInfos_Account` Is Null Or `deleteInfos_Account` = 0");

    if (!result)
    {
        TC_LOG_INFO("modules", ">> 加载了 0 条角色数据. 数据库表 `characters` 是空的.");
        return;
    }

    uint32 count = 0;

    do
    {
        Field* fields = result->Fetch();

        uint32 index = 0;
        uint32 guid      = fields[index].GetUInt32();
        uint32 accountId = fields[++index].GetUInt32();

        //m_allAccount[accountId].push_back(ObjectGuid(HighGuid::Player, guid));
        m_allAccount[accountId].emplace_back(HighGuid::Player, guid);

        ++count;
    } while (result->NextRow());

    TC_LOG_INFO("modules", ">> 从 `characters` 表中加载 %u 条数据花费 %u 毫秒", count, GetMSTimeDiffToNow(oldMSTime));
}

//void FakePlayerMgr::LoadOfflineAccount()
//{
//}
