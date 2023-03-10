#include "BotRogueAI.h"
#include "Item.h"
#include "Bag.h"
#include "Spell.h"
#include "Group.h"
#include "SpellHistory.h"
#include "WorldSession.h"

void BotRogueAI::InitializeSpells()
{
    RogueGuard_Sneak = FindMaxRankSpellByExist(1784);
    RogueGuard_ShadowCloak = FindMaxRankSpellByExist(31224);
    RogueGuard_Disappear = FindMaxRankSpellByExist(26889);
    RogueGuard_Dodge = FindMaxRankSpellByExist(26669);
    RogueGuard_Sprint = FindMaxRankSpellByExist(11305);

    RogueSneak_Stick = FindMaxRankSpellByExist(51724);
    RogueSneak_Premeditate = FindMaxRankSpellByExist(14183);
    RogueSneak_Ambush = FindMaxRankSpellByExist(48691);
    RogueSneak_Surprise = FindMaxRankSpellByExist(1833);

    RogueAssist_ShadowDance = FindMaxRankSpellByExist(51713);
    RogueAssist_ShadowFlash = FindMaxRankSpellByExist(36554);
    RogueAssist_ReadyCD = FindMaxRankSpellByExist(14185);
    RogueAssist_Blind = FindMaxRankSpellByExist(2094);
    RogueAssist_Disarm = FindMaxRankSpellByExist(51722);
    RogueAssist_NextCrit = FindMaxRankSpellByExist(14177);
    RogueAssist_blood = FindMaxRankSpellByExist(51662);
    RogueAssist_FastEnergy = FindMaxRankSpellByExist(13750);
    RogueAssist_BlockCast = FindMaxRankSpellByExist(1766);
    RogueAssist_Paralyze = FindMaxRankSpellByExist(1776);
    RogueAssist_FastSpeed = FindMaxRankSpellByExist(13877);

    RogueAOE_Knife = FindMaxRankSpellByExist(51723);
    RogueAOE_AllDance = FindMaxRankSpellByExist(51690);

    RogueAttack_Blood = FindMaxRankSpellByExist(48660);
    RogueAttack_Ghost = FindMaxRankSpellByExist(14278);
    RogueAttack_Injure = FindMaxRankSpellByExist(48666);
    RogueAttack_PoisonAtt = FindMaxRankSpellByExist(5938);
    RogueAttack_BackAtt = FindMaxRankSpellByExist(48657);
    RogueAttack_EvilAtt = FindMaxRankSpellByExist(48638);

    RogueAttack_Damage = FindMaxRankSpellByExist(48668);
    RogueAttack_Separate = FindMaxRankSpellByExist(48672);
    RogueAttack_Stun = FindMaxRankSpellByExist(8643);
    RogueAttack_PoisonDmg = FindMaxRankSpellByExist(57993);
    RogueRange_Throw = FindMaxRankSpellByExist(48674);

    RogueFlag_Dance = 51713;
}

void BotRogueAI::UpdateTalentType()
{
    m_BotTalentType = FindTalentType(me);// PlayerBotSetting::FindPlayerTalentType(me);
}

void BotRogueAI::ResetBotAI()
{
    BotBGAI::ResetBotAI();
    m_IsUpedPoison = 0;
    UpdateTalentType();
    InitializeSpells();
}

uint32 BotRogueAI::GetEnergyPowerPer()
{
    float per = (float)me->GetPower(POWER_ENERGY) / (float)me->GetMaxPower(POWER_ENERGY);
    return (uint32)(per * 100);
}

uint32 BotRogueAI::GetPoisonEntryByWeaponType(EquipmentSlots equipSlot)
{
    if (equipSlot == EQUIPMENT_SLOT_MAINHAND)
    {
        uint32 level = me->GetLevel();
        if (level < 30)
            return 6947;
        else if (level < 38)
            return 2892;
        else if (level < 46)
            return 2893;
        else if (level < 54)
            return 8984;
        else if (level < 60)
            return 8985;
        else if (level < 70)
            return 20844;
        else if (level < 80)
            return 22054;
        else
            return 43233;
    }
    else if (equipSlot == EQUIPMENT_SLOT_OFFHAND)
    {
        return 3775;
    }
    return 0;
}

void BotRogueAI::EachTick()
{
    BotBGAI::EachTick();
    if (!me->IsInCombat())
        return;
    UpEnergy();
}

void BotRogueAI::ProcessReady()
{
    if (m_IsUpedPoison >= 2)
        return;
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;
    ++m_IsUpedPoison;
    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    {
        if (Bag* pBag = me->GetBagByPos(i))
        {
            for (uint32 j = 0; j < pBag->GetBagSize(); j++)
            {
                Item* pItem = pBag->GetItemByPos(uint8(j));
                if (!pItem)
                    continue;
                me->DestroyItem(i, uint8(j), true);
            }
        }
    }
    EquipmentSlots equipSlot = (m_IsUpedPoison <= 1) ? EquipmentSlots::EQUIPMENT_SLOT_MAINHAND : EquipmentSlots::EQUIPMENT_SLOT_OFFHAND;
    Item* pWeaponItem = me->GetItemByPos(255, equipSlot);
    if (!pWeaponItem)
        return;
    ItemTemplate const* pTemplate = sObjectMgr->GetItemTemplate(GetPoisonEntryByWeaponType(equipSlot));
    if (!pTemplate)
        return;
    uint32 count = 2;
    uint32 noSpaceForCount = 0;
    ItemPosCountVec dest;
    InventoryResult msg = me->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, pTemplate->ItemId, count, &noSpaceForCount);
    if (msg != EQUIP_ERR_OK)
        count -= noSpaceForCount;
    if (count <= 0 || dest.empty())
        return;
    Item* itemInst = me->StoreNewItem(dest, pTemplate->ItemId, true, GenerateItemRandomPropertyId(pTemplate->ItemId));
    if (!itemInst)
        return;

    SpellCastTargets targets;
    targets.SetTargetMask(TARGET_FLAG_ITEM);
    targets.SetItemTarget(pWeaponItem);
    targets.SetSrc(me->GetPosition());
    targets.SetDst(me->GetPosition());
    targets.Update(me);
    me->CastItemUseSpell(itemInst, targets, m_IsUpedPoison, 0);
}

bool BotRogueAI::ProcessNormalSpell()
{
    Unit* pUnit = me->GetSelectedUnit();
    if (pUnit && pUnit->IsAlive())
    {
        if (!me->IsInCombat() && TryCastSpell(RogueGuard_Sneak, me) == SpellCastResult::SPELL_CAST_OK)
        {
            OnCastSneak();
            return false;
        }
    }
    else
    {
        if (me->HasAura(RogueGuard_Sneak))
        {
            me->RemoveOwnedAura(RogueGuard_Sneak, ObjectGuid::Empty, 0, AURA_REMOVE_BY_CANCEL);
        }
    }
    if (!me->HasAura(RogueGuard_Sneak) && TryUpMount())
        return true;
    return false;
}

bool BotRogueAI::ProcessSneakSpell(Unit* pTarget)
{
    if (!pTarget || !me->HasAura(RogueGuard_Sneak))
        return false;
    Unit* pNoSappedUnit = HasAuraMechanic(pTarget, Mechanics::MECHANIC_SAPPED) ? NULL : pTarget;
    NearUnitVec& enemys = RangeEnemyListByHasAura(0, NEEDFLEE_CHECKRANGE);
    for (Unit* player : enemys)
    {
        if (player == pTarget || player->IsInCombat())
            continue;
        if (HasAuraMechanic(player, Mechanics::MECHANIC_SAPPED))
            continue;
        if (TryCastSpell(RogueSneak_Stick, player) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (!pNoSappedUnit)
            pNoSappedUnit = player;
    }

    if (pNoSappedUnit && pNoSappedUnit != pTarget)
    {
        me->SetSelection(pNoSappedUnit->GetGUID());
        pTarget = pNoSappedUnit;
    }
    if (RogueSneak_Premeditate && TryCastSpell(RogueSneak_Premeditate, pTarget) == SpellCastResult::SPELL_CAST_OK)
        return true;
    if (!HasAuraMechanic(pTarget, Mechanics::MECHANIC_STUN))
    {
        if (TryCastSpell(RogueSneak_Surprise, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    else
    {
        if (RogueSneak_Ambush && TryCastSpell(RogueSneak_Ambush, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (RogueAttack_BackAtt && TryCastSpell(RogueAttack_BackAtt, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (TryCastSpell(RogueSneak_Surprise, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    return false;
}

bool BotRogueAI::ProcessMeleeDance(Unit* pTarget)
{
    if (m_BotTalentType != 2)
        return false;
    if (!CanBlind(pTarget))
        return false;
    //if (BotUtility::ControllSpellDiminishing && !TargetIsNotDiminishingByType(pTarget, DiminishingGroup::DIMINISHING_CONTROLLED_STUN))
    //	return false;
    if (me->HasAura(RogueFlag_Dance))
    {
        if (TryCastSpell(RogueSneak_Surprise, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    else if (RogueAssist_ShadowDance)
    {
        if (TryCastSpell(RogueAssist_ShadowDance, me) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (RogueAssist_ReadyCD && TryCastSpell(RogueAssist_ReadyCD, me) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    return false;
}

bool BotRogueAI::ProcessMeleeBlind(Unit* pTarget)
{
    NearUnitVec& enemys = RangeEnemyListByHasAura(0, BOTAI_TOTEMRANGE);
    for (Unit* player : enemys)
    {
        if (player == pTarget || !CanBlind(player))
            continue;
        NearUnitVec& friends = SearchFriendTargetIsTarget(player);
        if (!friends.empty())
            continue;
        //if (!BotUtility::ControllSpellDiminishing || TargetIsNotDiminishingByType(player, DiminishingGroup::DIMINISHING_DISORIENT))
        {
            if (TryCastSpell(RogueAssist_Paralyze, player) == SpellCastResult::SPELL_CAST_OK)
                return true;
        }
        //if (!BotUtility::ControllSpellDiminishing || TargetIsNotDiminishingByType(player, DiminishingGroup::DIMINISHING_FEAR))
        {
            if (!m_NeedReserveCtrlSpell && TryCastSpell(RogueAssist_Blind, player) == SpellCastResult::SPELL_CAST_OK)
                return true;
        }
    }
    return false;
}

void BotRogueAI::ProcessMeleeSpell(Unit* pTarget)
{
    if (me->GetSelectedUnit() && !me->IsInCombat() && !me->HasAura(RogueGuard_Sneak) &&
        TryCastSpell(RogueGuard_Sneak, me) == SpellCastResult::SPELL_CAST_OK)
    {
        OnCastSneak();
        return;
    }
    if (!CanStartSpell())
        return;
    if (ProcessSneakSpell(pTarget))
        return;
    NearUnitVec& enemys = RangeEnemyListByTargetIsMe(NEEDFLEE_CHECKRANGE);
    if (enemys.size() > 1 && TryCastSpell(RogueGuard_Dodge, me) == SpellCastResult::SPELL_CAST_OK)
        return;
    if (!me->InArena() && enemys.size() > 3)
    {
        if (RogueAOE_AllDance && TryCastSpell(RogueAOE_AllDance, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (RogueAOE_Knife && TryCastSpell(RogueAOE_Knife, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }

    if (pTarget->HasUnitState(UNIT_STATE_CASTING))
    {
        if (RogueAssist_BlockCast && TryCastSpell(RogueAssist_BlockCast, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (me->InArena())
        {
            if (RogueAssist_Paralyze && TryCastSpell(RogueAssist_Paralyze, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return;
            if (RogueAttack_Stun && TryCastSpell(RogueAttack_Stun, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return;
            //if (RogueAssist_Blind && TryCastSpell(RogueAssist_Blind, pTarget) == SpellCastResult::SPELL_CAST_OK)
            //	return;
        }
    }
    if (RogueAssist_Disarm && TargetIsMelee(pTarget->ToPlayer()) && !HasAuraMechanic(pTarget, Mechanics::MECHANIC_STUN))
    {
        if (!pTarget->HasAura(RogueAssist_Disarm) && TryCastSpell(RogueAssist_Disarm, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    if (me->InArena() && RogueAssist_Disarm)
    {
        NearUnitVec& nearEnemys = RangeEnemyListByHasAura(0, NEEDFLEE_CHECKRANGE);
        for (Unit* pUnit : nearEnemys)
        {
            if (Player* pPlayer = pUnit->ToPlayer())
            {
                if (!TargetIsMelee(pPlayer) || HasAuraMechanic(pPlayer, Mechanics::MECHANIC_STUN))
                    continue;
                if (pPlayer->HasAura(RogueAssist_Disarm))
                    continue;
                if (TryCastSpell(RogueAssist_Disarm, pPlayer) == SpellCastResult::SPELL_CAST_OK)
                    return;
            }
        }
    }
    if (CastCloakByNeed())
        return;
    if (ProcessMeleeBlind(pTarget))
        return;
    if (m_BotTalentType == 1 && RogueAssist_FastEnergy)
    {
        if (GetEnergyPowerPer() < 20 && TryCastSpell(RogueAssist_FastEnergy, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    if (RogueGuard_Disappear && me->InArena() && me->IsInCombat())// && !HasAuraMechanic(me, Mechanics::MECHANIC_BLEED))
    {
        if (!pTarget->HasAura(RogueAssist_Disarm) && !HasAuraMechanic(pTarget, Mechanics::MECHANIC_STUN) &&
            !HasAuraMechanic(pTarget, Mechanics::MECHANIC_FEAR))
        {
            if (TryCastSpell(RogueGuard_Disappear, me) == SpellCastResult::SPELL_CAST_OK)
            {
                OnCastSneak();
                return;
            }
        }
    }
    if (m_BotTalentType == 0 && RogueAssist_NextCrit)
        TryCastSpell(RogueAssist_NextCrit, pTarget);
    if (CanConsumeCombo(pTarget))
    {
        if (RogueAttack_Stun && !pTarget->HasAura(RogueAssist_Disarm) && !HasAuraMechanic(pTarget, Mechanics::MECHANIC_STUN))
        {
            //if (!BotUtility::ControllSpellDiminishing || TargetIsNotDiminishingByType(pTarget, DiminishingGroup::DIMINISHING_CONTROLLED_STUN))
            {
                if (TryCastSpell(RogueAttack_Stun, pTarget) == SpellCastResult::SPELL_CAST_OK)
                    return;
            }
        }
        if (ProcessMeleeDance(pTarget))
            return;
        if (RogueAttack_PoisonDmg && pTarget->HasAuraState(AuraStateType::AURA_STATE_DEADLY_POISON, NULL, me))
        {
            if (TryCastSpell(RogueAttack_PoisonDmg, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return;
        }
        if (pTarget->HasAura(RogueAttack_Separate, me->GetGUID()))
        {
            if (TryCastSpell(RogueAttack_Damage, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return;
        }
        else
        {
            if (TryCastSpell(RogueAttack_Separate, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return;
        }
    }
    else
    {
        if (RogueAttack_PoisonAtt && !HasAuraMechanic(pTarget, Mechanics::MECHANIC_SNARE))
        {
            if (TryCastSpell(RogueAttack_PoisonAtt, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return;
        }
        if (ProcessMeleeDance(pTarget))
            return;
        if (m_BotTalentType == 0 && RogueAttack_Injure && TryCastSpell(RogueAttack_Injure, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (m_BotTalentType == 2 && RogueAttack_Ghost && TryCastSpell(RogueAttack_Ghost, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (m_BotTalentType == 2 && RogueAttack_Blood)
        {
            if (TryCastSpell(RogueAttack_Blood, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return;
        }
        else if (m_BotTalentType != 0 && RogueAttack_BackAtt && TryCastSpell(RogueAttack_BackAtt, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (TryCastSpell(RogueAttack_EvilAtt, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
}

void BotRogueAI::ProcessRangeSpell(Unit* pTarget)
{
    if (me->GetSelectedUnit() != pTarget)
        return;
    if (me->GetSelectedUnit() && !me->IsInCombat() && TryCastSpell(RogueGuard_Sneak, me) == SpellCastResult::SPELL_CAST_OK)
    {
        OnCastSneak();
        return;
    }
    if (ProcessSneakSpell(pTarget))
        return;
    //if (ProcessMeleeDance(pTarget))
    //	return;
    if (RogueAssist_ShadowFlash)
    {
        if (me->HasAura(RogueGuard_Sneak) || (!HasAuraMechanic(me, Mechanics::MECHANIC_ROOT) && !HasAuraMechanic(me, Mechanics::MECHANIC_STUN)))
        {
            if (TryCastSpell(RogueAssist_ShadowFlash, pTarget) == SpellCastResult::SPELL_CAST_OK)
            {
                OnCastFlash(pTarget);
                return;
            }
        }
    }
    uint8 combo = me->GetComboPoints();
    if (RogueRange_Throw && pTarget->HasUnitState(UNIT_STATE_CASTING) && combo > 0)
    {
        if (TryCastSpell(RogueRange_Throw, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    else if (RogueRange_Throw && combo > 2)
    {
        if (TryCastSpell(RogueRange_Throw, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    if (CastCloakByNeed())
        return;
}

void BotRogueAI::ProcessFlee()
{
    FleeMovement();
    if (!me->IsInCombat() && TryCastSpell(RogueGuard_Sneak, me) == SpellCastResult::SPELL_CAST_OK)
    {
        OnCastSneak();
        return;
    }
    if (me->HasAura(RogueGuard_Sneak))
        return;

    if (CastCloakByNeed())
        return;
    NearUnitVec& enemys = RangeEnemyListByTargetIsMe(NEEDFLEE_CHECKRANGE);
    for (Unit* player : enemys)
    {
        if (!m_NeedReserveCtrlSpell && !HasAuraMechanic(player, Mechanics::MECHANIC_CHARM) && !HasAuraMechanic(player, Mechanics::MECHANIC_DISORIENTED))
        {
            if (TryCastSpell(RogueAssist_Blind, player) == SpellCastResult::SPELL_CAST_OK)
                return;
        }
        if (!HasAuraMechanic(player, Mechanics::MECHANIC_KNOCKOUT))
        {
            if (TryCastSpell(RogueAssist_Paralyze, player) == SpellCastResult::SPELL_CAST_OK)
                return;
        }
        if (me->HasAura(RogueFlag_Dance) && !HasAuraMechanic(player, Mechanics::MECHANIC_STUN))
        {
            if (TryCastSpell(RogueSneak_Surprise, player) == SpellCastResult::SPELL_CAST_OK)
                return;
        }
    }
    if (enemys.size() > 1)
    {
        if (TryCastSpell(RogueGuard_Sprint, me) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (TryCastSpell(RogueGuard_Dodge, me) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (!HasAuraMechanic(me, Mechanics::MECHANIC_BLEED))
        {
            if (TryCastSpell(RogueGuard_Disappear, me) == SpellCastResult::SPELL_CAST_OK)
            {
                me->AttackStop();
                return;
            }
        }
    }
}

bool BotRogueAI::CastCloakByNeed()
{
    if (!RogueGuard_ShadowCloak)
        return false;
    if (HasAuraMechanic(me, Mechanics::MECHANIC_ROOT))
    {
        if (TryCastSpell(RogueGuard_ShadowCloak, me) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    if (HasAuraMechanic(me, Mechanics::MECHANIC_SNARE))
    {
        if (TryCastSpell(RogueGuard_ShadowCloak, me) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }

    return false;
}

bool BotRogueAI::CanBlind(Unit* pTarget)
{
    if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_KNOCKOUT) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_BLEED) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_BANISH) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_FEAR) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_ROOT) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_STUN) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_SLEEP) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_SAPPED))
        return false;
    return true;
}

bool BotRogueAI::TryStartControlCommand()
{
    if (m_CruxControlTarget == ObjectGuid::Empty)
        return false;
    if (!RogueAssist_Blind)
    {
        m_CruxControlTarget = ObjectGuid::Empty;
        return false;
    }
    Player* pTarget = ObjectAccessor::FindPlayer(m_CruxControlTarget);
    if (!pTarget || !TargetIsNotDiminishingByType2(pTarget, DIMINISHING_FEAR) || !pTarget->IsAlive() || TargetIsControl(pTarget))
    {
        m_CruxControlTarget = ObjectGuid::Empty;
        return false;
    }
    NearUnitVec& friends = SearchFriend();
    for (Unit* pUnit : friends)
    {
        Player* pPlayer = pUnit->ToPlayer();
        if (!pPlayer)
            continue;
        if (BotBGAI* pAI = dynamic_cast<BotBGAI*>(pPlayer))
        {
            if (pAI->CastingControlCommandSpell())
            {
                m_CruxControlTarget = ObjectGuid::Empty;
                return false;
            }
        }
    }
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(RogueAssist_Blind);
    if (!spellInfo || spellInfo->IsPassive())
    {
        m_CruxControlTarget = ObjectGuid::Empty;
        return false;
    }
    if (me->GetSpellHistory()->HasGlobalCooldown(spellInfo))
        return true;
    if (!me->GetSpellHistory()->IsReady(spellInfo))
    {
        m_CruxControlTarget = ObjectGuid::Empty;
        return false;
    }

    if (me->IsWithinLOSInMap(pTarget) && me->GetDistance(pTarget) < BOTAI_TOTEMRANGE)
    {
        TryCastSpell(RogueAssist_Blind, pTarget);
    }
    else
    {
        if (!IsNotMovement())
            m_Movement.MovementTo(m_CruxControlTarget);
    }
    return true;
}

float BotRogueAI::TryPushControlCommand(Player* pTarget)
{
    if (!pTarget || !pTarget->IsAlive() || !pTarget->IsInWorld() || me->GetMap() != pTarget->GetMap())
    {
        ClearCruxControlCommand();
        return -1;
    }
    if (!RogueAssist_Blind)
        return -1;
    if (!TargetIsNotDiminishingByType2(pTarget, DIMINISHING_FEAR))
        return -1;
    if (!SpellHasReady(RogueAssist_Blind))
        return -1;
    m_CruxControlTarget = pTarget->GetGUID();
    m_LastControlTarget = m_CruxControlTarget;
    return me->GetDistance(pTarget->GetPosition());
}

bool BotRogueAI::CanConsumeCombo(Unit* pTarget)
{
    uint8 combo = me->GetComboPoints();
    if (combo == 0)
        return false;
    float targetLife = pTarget->GetHealthPct();
    if (targetLife < 10)
        return true;
    else if (targetLife < 60 && combo > 2)
        return true;
    else if (combo >= 4)
        return true;
    return false;
}

bool BotRogueAI::CanStartSpell()
{
    return GetEnergyPowerPer() >= 60;
}

void BotRogueAI::OnCastSneak()
{
    NearUnitVec& enemys = RangeEnemyListByTargetIsMe(BOTAI_SEARCH_RANGE);
    for (Unit* pUnit : enemys)
    {
        if (Player* player = pUnit->ToPlayer())
            player->SetSelection(ObjectGuid::Empty);
        else
            pUnit->SetTarget(ObjectGuid::Empty);
    }
}

void BotRogueAI::OnCastFlash(Unit* pTarget)
{
    me->GetMotionMaster()->Clear();
    Position& pos = pTarget->GetPosition();
    me->TeleportTo(me->GetMapId(), pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), me->GetOrientation());
    WorldSession* pSession = me->GetSession();
    WorldPacket opcode(2);
    opcode.appendPackGUID(me->GetGUID().GetRawValue());
    opcode << uint32(0);
    opcode << uint32(100);
    pSession->HandleMoveTeleportAck(opcode);
    m_Movement.SyncPosition(pos, true);
}

void BotRogueAI::UpEnergy()
{
    uint32 max = me->GetMaxPower(Powers::POWER_ENERGY);
    uint32 power = me->GetPower(Powers::POWER_ENERGY);
    if (me->InArena())
    {
        Group* pGroup = me->GetGroup();
        if (pGroup)
        {
            Group::MemberSlotList const& memList = pGroup->GetMemberSlots();
            for (Group::MemberSlot const& slot : memList)
            {
                Player* player = ObjectAccessor::FindPlayer(slot.guid);
                if (!player || me->GetMap() != player->GetMap() || !player->IsInWorld() || player->IsPlayerBot())
                    continue;
                me->SetPower(Powers::POWER_ENERGY, (max / 24) + power);
                return;
            }
        }
        else
            return;
        me->SetPower(Powers::POWER_ENERGY, (max / 24) + power);
    }
    else
        me->SetPower(Powers::POWER_ENERGY, (max / 30) + power);
}
