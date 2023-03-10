#include "PlayerBotAI.h"
#include "PlayerBotSetting.h"
#include "BotFieldAI.h"
#include "Battleground.h"
#include "Spell.h"
#include "Pet.h"

PlayerBotAI::PlayerBotAI(Player* player) :
    PlayerAI(player),
    m_AIType(AITYPE_NULL),
    //m_Movement(new BotAIMovement(player, this)),
    m_Movement(player, this),
    m_Teleporting(player)
{
}

PlayerBotAI::~PlayerBotAI()
{
    //if (m_Movement)
    //{
    //    delete m_Movement;
    //    m_Movement = nullptr;
    //}
}

PlayerBotAIType PlayerBotAI::GetTypeAI() const
{
    return (PlayerBotAIType)m_AIType;
}

void PlayerBotAI::SetTypeAI(PlayerBotAIType aiType)
{
    m_AIType = aiType;
}

void PlayerBotAI::SelectPlayerBotAI(PlayerBotAIType aiType, bool force)
{
    sFakePlayerMgr->SelectPlayerBotAI(me, aiType, force);
}

void PlayerBotAI::SelectAI()
{
    if (Battleground* bg = me->GetBattleground())
    {
        if (bg->isArena())
            SetTypeAI(AITYPE_ARENA);
        else
            SetTypeAI(AITYPE_BG);
    }
    else if(me->duel && me->duel->State != DUEL_STATE_COMPLETED && me->duel->Opponent)
        SetTypeAI(AITYPE_DUEL);
    else if(me->GetGroup())
        SetTypeAI(AITYPE_GROUP);
    else
        SetTypeAI(AITYPE_FIELD);
}

uint8 PlayerBotAI::FindTalentType(Player* player)
{
    return sFakePlayerMgr->FindTalentType(player);
}

bool PlayerBotAI::IsTankPlayer(Player* player)
{
    return sFakePlayerMgr->IsTankPlayer(player);
}

bool PlayerBotAI::IsHealPlayer(Player* player)
{
    return sFakePlayerMgr->IsHealPlayer(player);
}

bool PlayerBotAI::IsDpsPlayer(Player* player)
{
    return sFakePlayerMgr->IsDpsPlayer(player);
}

uint8 PlayerBotAI::CurrentTalentPoints(Player* player)
{
    return sFakePlayerMgr->CurrentTalentPoints(player);
}

uint32 PlayerBotAI::FindMaxRankSpellByExist(uint32 spellID, Player* player)
{
    if (spellID == 0)
        return 0;

    if (!player)
        player = me;

    uint32 selectSpell = sSpellMgr->GetLastSpellInChain(spellID);
    if (selectSpell == 0)
    {
        if (player->HasSpell(spellID))
            return spellID;
        return 0;
    }

    while (!player->HasSpell(selectSpell))
    {
        selectSpell = sSpellMgr->GetPrevSpellInChain(selectSpell);
        if (selectSpell == 0)
            return 0;
    }

    return selectSpell;
}

bool PlayerBotAI::SpellHasReady(uint32 spellID, Player* player)
{
    if (spellID == 0)
        return false;

    if (!player)
        player = me;

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
    if (!spellInfo || spellInfo->IsPassive())
        return false;
    if (player->GetSpellHistory()->HasGlobalCooldown(spellInfo))
        return false;
    if (player->GetSpellHistory()->IsReady(spellInfo))
        return true;
    return false;
}

bool PlayerBotAI::IsNotSelect(Unit* pTarget)
{
    if (!pTarget || !pTarget->IsAlive())
        return true;
    if (pTarget->HasAura(27827)) // (27827 救赎之魂 神牧死亡后)
        return true;
    return false;
}

void PlayerBotAI::TryTeleportHome()
{
    if (HasTeleport())
        return;

    if (me->GetMapId() == me->m_homebindMapId)
    {
        if (me->GetDistance(me->m_homebindX, me->m_homebindY, me->m_homebindZ) < 80)
            return;
    }

    Position homePosition = Position(me->m_homebindX, me->m_homebindY, me->m_homebindZ, me->GetOrientation());
    SetTeleport(me->m_homebindMapId, homePosition);
}

void PlayerBotAI::SetPetAllSpellAutocast(Pet* pet, bool autocast, uint32 excludeSpell/* = 0*/)
{
    if (!pet)
        return;

    for (PetSpellMap::iterator itrSpell = pet->m_spells.begin(); itrSpell != pet->m_spells.end(); ++itrSpell)
    {
        uint32 petSpellID = itrSpell->first;
        if (petSpellID == excludeSpell)
            continue;
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(petSpellID);
        if (!spellInfo || spellInfo->IsPassive() || !spellInfo->IsAutocastable())
            continue;
        pet->ToggleAutocast(spellInfo, (petSpellID == 1742) ? false : autocast);
        if (CharmInfo* charmInfo = pet->GetCharmInfo())
            charmInfo->SetSpellAutocast(spellInfo, (petSpellID == 1742) ? false : autocast);
    }
}

void PlayerBotAI::SetPetAllSpellAutocast(bool autocast, uint32 excludeSpell/* = 0*/)
{
    SetPetAllSpellAutocast(me->GetPet(), autocast, excludeSpell);
}

void PlayerBotAI::PlayerBotRoll(Group* group, Player* player, Roll const& roll)
{
    sFakePlayerMgr->PlayerBotRoll(group, player, roll);
}

bool PlayerBotAI::GiveAtGroupPos(Group* group, ObjectGuid& guid, uint32& index, uint32& count)
{
    return sFakePlayerMgr->GiveAtGroupPos(group, guid, index, count);
}

bool PlayerBotAI::GroupExistRealPlayer(Group* group)
{
    return sFakePlayerMgr->GroupExistRealPlayer(group);
}

bool PlayerBotAI::GroupExistPlayerBot(Group* group)
{
    return sFakePlayerMgr->GroupExistPlayerBot(group);
}

bool PlayerBotAI::AllGroupNotCombat(Group* group)
{
    return sFakePlayerMgr->AllGroupNotCombat(group);
}

bool PlayerBotAI::AllGroupIsIDLE(Group* group)
{
    return sFakePlayerMgr->AllGroupIsIDLE(group);
}

void PlayerBotAI::AllGroupBotGiveXP(Group* group, uint32 XP)
{
    sFakePlayerMgr->AllGroupBotGiveXP(group, XP);
}

Unit* PlayerBotAI::GetGroupTankTarget(Group* group)
{
    return sFakePlayerMgr->GetGroupTankTarget(group);
}

std::vector<ObjectGuid> PlayerBotAI::GetGroupMemberFromNeedRevivePlayer(Group* group, uint32 forMap)
{
    return sFakePlayerMgr->GetGroupMemberFromNeedRevivePlayer(group, forMap);
}

void PlayerBotAI::ResetRaidDungeon(Group* group)
{
    sFakePlayerMgr->ResetRaidDungeon(group);
}

void PlayerBotAI::ClearAllGroupForceFleeState(Group* group)
{
    sFakePlayerMgr->ClearAllGroupForceFleeState(group);
}

void PlayerBotAI::ProcessGroupBotCommand(Group* group, Player* srcPlayer, std::string& cmd)
{
    sFakePlayerMgr->ProcessGroupBotCommand(group, srcPlayer, cmd);
}

void PlayerBotAI::OnLeaderChangePhase(Group* group, Player* changeTarget, uint32 newPhase)
{
    sFakePlayerMgr->OnLeaderChangePhase(group, changeTarget, newPhase);
}

Creature* PlayerBotAI::SearchSeduceCreature(Player* centerPlayer)
{
    return sFakePlayerMgr->SearchSeduceCreature(centerPlayer);
}

BotGroupAI* PlayerBotAI::SearchExecuteSeduceBotAI(Group* group)
{
    return sFakePlayerMgr->SearchExecuteSeduceBotAI(group);
}

void PlayerBotAI::Reset()
{
    PlayerAI::Reset();
    ResetBotAI();
}

void PlayerBotAI::UpdateAI(uint32 diff)
{
    UpdateBotAI(diff);
}
