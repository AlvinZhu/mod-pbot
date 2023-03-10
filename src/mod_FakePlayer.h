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

#ifndef SC_FAKEPLAYERMGR_H
#define SC_FAKEPLAYERMGR_H

#include "ScriptMgr.h"
#include "TaskScheduler.h"

class PlayerBotAI;
class BotGroupAI;
namespace lfg
{
    class LfgPlayerData;
}
enum PlayerBotAIType : uint8;

enum FakePlayerEvents : uint8
{
    EVENT_NONE = 0,

    EVENT_UPDATE_ACCOUNT,
    EVENT_CHECK_PLAYERCOUNT,
    EVENT_REMOVE_ALL,

    EVENT_LFG_QUEUE_COOLDOWN,
    EVENT_BATTLEGROUND_QUEUE_COOLDOWN,
    EVENT_LFG_QUEUE_CHECK,
    EVENT_BATTLEGROUND_QUEUE_CHECK,

    EVENT_HANDLE_LFG_ROLE_CHECK,
    EVENT_HANDLE_LFG_PROPOSAL,
};

struct PlayerBotTalent
{
public:
    PlayerBotTalent() : MaxPageIndex(0), MaxPagePoint(0)
    {
        memset(PageCount, 0, sizeof(PageCount));
    }

public:
    uint32 PageCount[MAX_TALENT_TABS];
    uint32 MaxPageIndex;
    uint32 MaxPagePoint;
};

class FakePlayerMgr
{
public:
    static FakePlayerMgr* instance();

    typedef std::set<uint32> AccountSet;
    typedef std::vector<ObjectGuid> PlayerGuidList;
    typedef std::unordered_map<uint32, PlayerGuidList> AccountContainer;
    typedef std::unordered_map<ObjectGuid, Player*> PlayerContainer;
    typedef std::unordered_map<ObjectGuid, ObjectGuid> PlayerGuidContainer;
    typedef std::unordered_map<uint64, PlayerBotTalent> PlayerTalentContainer;
    AccountContainer m_allAccount;
    AccountContainer m_botAccount;
    PlayerContainer m_botPlayers;
    PlayerGuidContainer m_lastWhispers;
    PlayerGuidContainer m_gmFakePlayers;
    PlayerTalentContainer m_playerTalents;
    GuidSet m_QueueLfgCooldownStore;
    GuidSet m_QueueBgCooldownStore;

    bool Enable;
    //bool LoginMessage;
    uint32 AccountMax;
    AccountSet AccountAllow;
    AccountSet AccountIgnore;
    uint32 LoginMinTime;
    uint32 LoginMaxTime;
    double OnlineRate;
    uint32 OnlineMin;
    uint32 OnlineMax;
    bool MessageToGameMaster;
    AccountSet MessageGameMasters;

    // ----- 机器人设置 ----- //
    uint32 QueueLfgMinTime;
    uint32 QueueLfgMaxTime;
    uint32 QueueBgMinTime;
    uint32 QueueBgMaxTime;

    bool FieldCreature;
    bool FieldPVP;

    EventMap events;
    TaskScheduler scheduler;

    FakePlayerMgr();

    void LoadConfig(bool reload);

    uint32 GetFakePlayerCount() const;
    uint32 GetPlayerCount() const;
    void CheckPlayerCount();

    uint32 GetPlayerBotAccount(ObjectGuid& playerGuid);
    bool RemovePlayerBot(ObjectGuid playerGuid);
    bool RemoveAccountBot(uint32 accountId, bool all = false);
    void HandlePlayerLoginOpcode(ObjectGuid& playerGuid);
    bool FakePlayerLogin();
    bool FakePlayerLogout(bool check = false);

    void CleanData(bool all = false);
    void RemoveAllPlayerBot();
    void LoadAllAccountPlayer();

    bool IsBotPlayer(ObjectGuid& playerGuid);
    bool IsBotPlayer(Player* player);

    // ----- 天赋函数 ----- //
    uint8 FindTalentType(Player* player);
    bool IsTankPlayer(Player* player);
    bool IsHealPlayer(Player* player);
    bool IsDpsPlayer(Player* player);
    uint8 CurrentTalentPoints(Player* player);
    void ClearStorePlayerTalent(Player* player);

    // ----- 团队函数 ----- //
    void PlayerBotRoll(Group* group, Player* player, Roll const& roll);
    bool GiveAtGroupPos(Group* group, ObjectGuid& guid, uint32& index, uint32& count);
    bool GroupExistRealPlayer(Group* group);
    bool GroupExistPlayerBot(Group* group);
    bool AllGroupNotCombat(Group* group);
    bool AllGroupIsIDLE(Group* group);
    void AllGroupBotGiveXP(Group* group, uint32 XP);
    Unit* GetGroupTankTarget(Group* group);
    std::vector<ObjectGuid> GetGroupMemberFromNeedRevivePlayer(Group* group, uint32 forMap);
    void ResetRaidDungeon(Group* group);
    void ClearAllGroupForceFleeState(Group* group);
    void ProcessGroupBotCommand(Group* group, Player* srcPlayer, std::string& cmd);
    void OnLeaderChangePhase(Group* group, Player* changeTarget, uint32 newPhase);

    Creature* SearchSeduceCreature(Player* centerPlayer);
    BotGroupAI* SearchExecuteSeduceBotAI(Group* group);

    // ----- 地下城函数 ----- //
    void LfgQueueCooldown();
    void CheckLfgQueue();
    bool BotJoinLfgQueue(uint8 level, uint8 teamId, lfg::LfgPlayerData const* playerData);
    uint8 GenerateLfgRoles(Player* player);

    // ----- 战场函数 ----- //
    void BgQueueCooldown();
    void CheckBGQueue();

    // ----- 事件 ----- //
    void TaskSafe(Milliseconds time, uint32 eventId, Player* player, std::function<void(Player*)> const& callback);
    void TaskSafe(Milliseconds min, Milliseconds max, uint32 eventId, Player* player, std::function<void(Player*)> const& callback);
    void TaskOneSafe(Milliseconds min, Milliseconds max, uint32 eventId, Player* player, std::function<void(Player*)> const& callback);
    void TaskSafe(Milliseconds time, Player* player, std::function<void(Player*)> const& callback);
    void TaskSafe(Milliseconds min, Milliseconds max, Player* player, std::function<void(Player*)> const& callback);

    // 传送
    bool HandleTeleportAck(Player* bot);
    // 接受团队邀请
    bool HandleGroupInvite(Player* bot, WorldPacket& packet);
    // 接受复活
    bool HandleResurrect(Player* bot, WorldPacket& packet);
    // 加入地下城队列返回状态(成功/失败原因)
    bool HandleLFGJoinResult(Player* bot, WorldPacket& packet);
    // 确认随机本职务
    //bool HandleLFGRoleChosen(Player* bot, WorldPacket& packet);
    // 检查地下城职务
    bool HandleLFGRoleCheck(Player* bot, WorldPacket& packet);
    // 确认进入地下城检查
    bool HandleLFGProposal(Player* bot, WorldPacket& packet);
    // 接受决斗
    bool HandleDuelRequested(Player* bot, WorldPacket& packet);

    // ----- AI ----- //
    PlayerBotAI* GetPlayerBotAI(Player* bot);
    void SelectPlayerBotAI(Player* bot, PlayerBotAIType aiType, bool force = false);
    void SwitchPlayerBotAI(Player* bot, bool force = false);

private:
    //void LoadOfflineAccount();
};
#define sFakePlayerMgr FakePlayerMgr::instance()

#endif
