
#ifndef _PLAYERBOT_AI_H_
#define _PLAYERBOT_AI_H_

#include "SpellMgr.h"
#include "GridNotifiers.h"
#include "PlayerAI.h"
#include "mod_FakePlayer.h"
#include "BotAITool.h"
#include "BotAIMovement.h"

#define ARENA_PLAYER_BOT_AURA 80849
#define ARENA_WARRIOR_BOT_AURA 80850
#define ARENA_PALADIN_BOT_AURA 80851
#define ARENA_ROGUE_BOT_AURA 80852
#define ARENA_HUNTER_BOT_AURA 80853
#define ARENA_SHAMAN_BOT_AURA 80854
#define ARENA_MAGE_BOT_AURA 80855
#define ARENA_WARLOCK_BOT_AURA 80856
#define ARENA_PRIEST_BOT_AURA 80857
#define ARENA_DRUID_BOT_AURA 80858

#define BOTAI_UPDATE_TICK 500

#define BOTAI_MAXTARGET_TICKTIME 30000
#define BOTAI_FIELDTELEPORT_DISTANCE 80
#define BOTAI_SEARCH_RANGE 32
#define BOTAI_RANGESPELL_DISTANCE 28
#define BOTAI_TOTEMRANGE 18
#define BOTAI_FLEE_JUDGE 14

#define NEEDFLEE_CHECKRANGE 10

enum PlayerBotAIType : uint8
{
    AITYPE_NULL,
    AITYPE_FIELD,
    AITYPE_GROUP,
    AITYPE_DUEL,
    AITYPE_BG,
    AITYPE_ARENA,
};

class BotBGAI;
class BotGroupAI;
class BotFieldAI;
class BotDuelAI;
class BotArenaAI;

class TC_GAME_API PlayerBotAI : public PlayerAI
{
public:
    PlayerBotAI(Player* player);
	~PlayerBotAI();

    virtual void ResetBotAI() {}
    void Reset() override;

    virtual void UpdateBotAI(uint32 diff) {}
    void UpdateAI(uint32 diff) override;

    PlayerBotAIType GetTypeAI() const;
    void SelectPlayerBotAI(PlayerBotAIType aiType, bool force);

    uint32 FindMaxRankSpellByExist(uint32 spellID, Player* player = nullptr);
    bool SpellHasReady(uint32 spellID, Player* player = nullptr);

    virtual bool IsNotSelect(Unit* pTarget);
    void SetTeleport(Position& telePos) { m_Teleporting.SetTeleport(telePos); }
    void SetTeleport(uint32 mapID, Position& telePos) { m_Teleporting.SetTeleport(mapID, telePos); }
    void SetTeleport(Player* pTarget, float offset = NEEDFLEE_CHECKRANGE) { m_Teleporting.SetTeleport(pTarget, offset); }
    bool HasTeleport() { return !m_Teleporting.CanMovement(); }
    void UpdateTeleport(uint32 diff) { m_Teleporting.Update(diff, &m_Movement); }
    void TryTeleportHome();

    void SetPetAllSpellAutocast(Pet* pet, bool autocast, uint32 excludeSpell = 0);
    void SetPetAllSpellAutocast(bool autocast, uint32 excludeSpell = 0);

    // ----- 天赋函数 ----- //
    uint8 FindTalentType(Player* player);
    bool IsTankPlayer(Player* player);
    bool IsHealPlayer(Player* player);
    bool IsDpsPlayer(Player* player);
    uint8 CurrentTalentPoints(Player* player);

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

    BotAIMovement* GetAIMovement() { return &m_Movement; }

    BotBGAI* ToBotBGAI() { if (GetTypeAI() == AITYPE_BG) return reinterpret_cast<BotBGAI*>(this); else return nullptr; }
    BotBGAI const* ToBotBGAI() const { if (GetTypeAI() == AITYPE_BG) return reinterpret_cast<BotBGAI const*>(this); else return nullptr; }

    BotGroupAI* ToBotGroupAI() { if (GetTypeAI() == AITYPE_GROUP) return reinterpret_cast<BotGroupAI*>(this); else return nullptr; }
    BotGroupAI const* ToBotGroupAI() const { if (GetTypeAI() == AITYPE_GROUP) return reinterpret_cast<BotGroupAI const*>(this); else return nullptr; }

    BotFieldAI* ToBotFieldAI() { if (GetTypeAI() == AITYPE_FIELD) return reinterpret_cast<BotFieldAI*>(this); else return nullptr; }
    BotFieldAI const* ToBotFieldAI() const { if (GetTypeAI() == AITYPE_FIELD) return reinterpret_cast<BotFieldAI const*>(this); else return nullptr; }

    BotDuelAI* ToBotDuelAI() { if (GetTypeAI() == AITYPE_DUEL) return reinterpret_cast<BotDuelAI*>(this); else return nullptr; }
    BotDuelAI const* ToBotDuelAI() const { if (GetTypeAI() == AITYPE_DUEL) return reinterpret_cast<BotDuelAI const*>(this); else return nullptr; }

    BotArenaAI* ToBotArenaAI() { if (GetTypeAI() == AITYPE_ARENA) return reinterpret_cast<BotArenaAI*>(this); else return nullptr; }
    BotArenaAI const* ToBotArenaAI() const { if (GetTypeAI() == AITYPE_ARENA) return reinterpret_cast<BotArenaAI const*>(this); else return nullptr; }

protected:
    void SetTypeAI(PlayerBotAIType aiType);
    void SelectAI();

    BotAIMovement m_Movement;

    BotAITeleport m_Teleporting;

private:
    uint8 m_AIType;
};

#endif // !_PLAYERBOT_AI_H_
