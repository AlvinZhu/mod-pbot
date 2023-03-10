#include "BotHunterAI.h"
#include "Pet.h"
#include "WorldSession.h"

void BotHunterAI::InitializeSpells()
{
    HunterIDLE_SummonPet = FindMaxRankSpellByExist(883);
    HunterIDLE_RevivePet = FindMaxRankSpellByExist(982);
    HunterIDLE_ManaAura = FindMaxRankSpellByExist(34074);
    HunterIDLE_DodgeAura = FindMaxRankSpellByExist(13163);
    HunterIDLE_EagleAura = FindMaxRankSpellByExist(27044);
    HunterIDLE_DragonAura = FindMaxRankSpellByExist(61847);
    HunterIDLE_ShotAura = FindMaxRankSpellByExist(19506);

    HunterTrap_FarFrozen = FindMaxRankSpellByExist(60192);
    HunterTrap_Frozen = FindMaxRankSpellByExist(14311);
    HunterTrap_Ice = FindMaxRankSpellByExist(13809);
    HunterTrap_Viper = FindMaxRankSpellByExist(34600);
    HunterTrap_Explode = FindMaxRankSpellByExist(49067);
    HunterTrap_Fire = FindMaxRankSpellByExist(49056);
    HunterTrap_Shot = FindMaxRankSpellByExist(63672);

    HunterAssist_ClearRoot = FindMaxRankSpellByExist(53271);
    HunterAssist_PetCommand = FindMaxRankSpellByExist(34026);
    HunterAssist_HealPet = FindMaxRankSpellByExist(48990);
    HunterAssist_PetStun = FindMaxRankSpellByExist(19577);
    HunterAssist_PetRage = FindMaxRankSpellByExist(19574);
    HunterAssist_Stamp = FindMaxRankSpellByExist(53338);
    HunterAssist_FalseDead = FindMaxRankSpellByExist(5384);
    HunterAssist_BackJump = FindMaxRankSpellByExist(781);
    HunterAssist_FastSpeed = FindMaxRankSpellByExist(3045);
    HunterAssist_ReadyCD = FindMaxRankSpellByExist(23989);

    HunterMelee_BackRoot = FindMaxRankSpellByExist(48999);
    HunterMelee_NoDamage = FindMaxRankSpellByExist(19263);
    HunterMelee_DecSpeed = FindMaxRankSpellByExist(2974);
    HunterMelee_NextAtt = FindMaxRankSpellByExist(48996);
    HunterMelee_MeleeAtt = FindMaxRankSpellByExist(53339);

    HunterDebug_Damage = FindMaxRankSpellByExist(49001);
    HunterDebug_Mana = FindMaxRankSpellByExist(3034);
    HunterDebug_Sleep = FindMaxRankSpellByExist(49012);

    HunterShot_AOEShot = FindMaxRankSpellByExist(58434);
    HunterShot_CharmShot = FindMaxRankSpellByExist(19503);
    HunterShot_Explode = FindMaxRankSpellByExist(60053);
    HunterShot_Aim = FindMaxRankSpellByExist(49050);
    HunterShot_Silence = FindMaxRankSpellByExist(34490);
    HunterShot_Shock = FindMaxRankSpellByExist(5116);
    HunterShot_Cast = FindMaxRankSpellByExist(49052);
    HunterShot_MgcShot = FindMaxRankSpellByExist(49045);
    HunterShot_KillShot = FindMaxRankSpellByExist(61006);
    HunterShot_MulShot = FindMaxRankSpellByExist(49048);
    HunterShot_QMLShot = FindMaxRankSpellByExist(53209);
}

void BotHunterAI::UpdateTalentType()
{
    m_BotTalentType = FindTalentType(me);// PlayerBotSetting::FindPlayerTalentType(me);
}

void BotHunterAI::ResetBotAI()
{
    BotBGAI::ResetBotAI();
    m_IsSupplemented = false;
    m_IsReviveManaModel = false;
    UpdateTalentType();
    InitializeSpells();
    SetPetAllSpellAutocast(true);
    if (HunterShot_QMLShot == 0 && m_BotTalentType == 1 && me->GetLevel() == 80)
    {
        me->LearnSpell(53209, false);
        HunterShot_QMLShot = FindMaxRankSpellByExist(53209);
    }
}

uint32 BotHunterAI::GetManaPowerPer()
{
    float per = (float)me->GetPower(POWER_MANA) / (float)me->GetMaxPower(POWER_MANA);
    return (uint32)(per * 100);
}

bool BotHunterAI::NeedFlee()
{
    if (m_Flee.Fleeing())
        return true;
    NearUnitVec& nearEnemys = RangeEnemyListByTargetIsMe(NEEDFLEE_CHECKRANGE);
    if (me->InArena())
    {
        for (Unit* pUnit : nearEnemys)
        {
            if (m_NeedFlee.TargetHasFleeAura(pUnit))
            {
                me->SetSelection(pUnit->GetGUID());
                return true;
            }
        }
    }
    else if (nearEnemys.size() > 0)
    {
        Unit* pNear = nearEnemys[urand(0, nearEnemys.size() - 1)];
        me->SetSelection(pNear->GetGUID());
        return true;
    }
    Unit* pTarget = me->GetSelectedUnit();
    if (!pTarget)
        return false;
    if (me->InArena() && !IsFleeTargetByRangeBot(pTarget))
        return false;
    float fleeDistance = m_Flee.CalcMaxFleeDistance(pTarget);
    if (me->GetDistance(pTarget->GetPosition()) < fleeDistance)//BOTAI_FLEE_JUDGE)
        return true;
    return false;
}

void BotHunterAI::ProcessReady()
{
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return;

    if (!m_IsSupplemented)
    {
        m_IsSupplemented = true;
#ifdef CommandBG
        me->SupplementAmmo();
#endif
        return;
    }
    if (me->GetPetGUID().IsEmpty())
    {
        TC_LOG_INFO("BotBGAI", "Hunter Ai check pet, is no pet!");
#ifdef CommandBG
        PlayerBotSetting::CheckHunterPet(me);
#endif
        return;
    }
    ProcessNormalSpell();
}

void BotHunterAI::ProcessFlee()
{
    FleeMovement();

    Unit* pSelectTarget = me->GetSelectedUnit();
    NearUnitVec& enemys = RangeEnemyListByTargetIsMe(NEEDFLEE_CHECKRANGE);
    if (enemys.empty())
        return;
    if (!pSelectTarget && !enemys.empty())
        pSelectTarget = enemys[urand(0, enemys.size() - 1)];
    if (!pSelectTarget)
        return;
    Pet* pPet = me->GetPet();
    if (TargetIsSuppress(pSelectTarget))
    {
        if (pPet)
        {
            pPet->AttackStop();
            pPet->SetTarget(ObjectGuid::Empty);
        }
        me->AttackStop();
        return;
    }
    if (pPet)
    {
        Unit* pPetTarget = pPet->GetVictim();
        if (pPetTarget && pPetTarget != pSelectTarget)
            PetAction(pPet, pSelectTarget);
    }

    if (enemys.size() > 1 && TryCastSpell(HunterTrap_Ice, me) == SpellCastResult::SPELL_CAST_OK)
        return;
    else if (TryCastSpell(HunterTrap_Frozen, me) == SpellCastResult::SPELL_CAST_OK)
        return;
    if (ProcessAura(true))
        return;
    if (enemys.size() > 1 && TryCastSpell(HunterMelee_NoDamage, me) == SpellCastResult::SPELL_CAST_OK)
        return;
    bool hasRoot = HasRootMechanic();
    if (hasRoot && HunterAssist_ClearRoot && TryCastSpell(HunterAssist_ClearRoot, me) == SpellCastResult::SPELL_CAST_OK)
        return;
    if (enemys.size() > 1 && TryCastSpell(HunterAssist_FalseDead, me) == SpellCastResult::SPELL_CAST_OK)
        return;

    Unit* pCastEnemy = RandomRangeEnemyByCasting(BOTAI_RANGESPELL_DISTANCE);
    if (pCastEnemy)
    {
        if (m_BotTalentType == 1 && HunterShot_Silence && TryCastSpell(HunterShot_Silence, pCastEnemy) == SpellCastResult::SPELL_CAST_OK)
            return;
        else if (m_BotTalentType == 2 && HunterShot_CharmShot && TryCastSpell(HunterShot_CharmShot, pCastEnemy) == SpellCastResult::SPELL_CAST_OK)
            return;
    }

    if (m_BotTalentType == 0)
    {
        if (hasRoot && HunterAssist_PetRage && TryCastSpell(HunterAssist_PetRage, me) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (TryCastSpell(HunterAssist_PetStun, pSelectTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    if (CastMeleeSpell(pSelectTarget))
        return;
    if (CastRangeSpell(pSelectTarget))
        return;
    if (me->InArena())
    {
        uint32 minLifePct = 100;
        Unit* pMinUnit = NULL;
        Unit* pMeleeUnit = NULL;
        NearUnitVec& enemys = RangeEnemyListByHasAura(0, BOTAI_RANGESPELL_DISTANCE);
        for (Unit* pUnit : enemys)
        {
            if (pUnit == pSelectTarget || TargetIsSuppress(pUnit))
                continue;
            float dist = me->GetDistance(pUnit->GetPosition());
            if (dist <= 9)
            {
                if (dist < 7)
                    pMeleeUnit = pUnit;
                continue;
            }
            uint32 lifePct = uint32(pUnit->GetHealthPct());
            if (!pMinUnit || lifePct < minLifePct)
            {
                pMinUnit = pUnit;
                minLifePct = lifePct;
            }
        }
        if (pMinUnit && CastRangeSpell(pMinUnit))
            return;
        if (pMeleeUnit && CastMeleeSpell(pMeleeUnit))
            return;
    }
}

bool BotHunterAI::ProcessNormalSpell()
{
    if (me->HasUnitState(UNIT_STATE_CASTING))
        return true;
    //if (me->GetPetGUID().IsEmpty() && CanCastSpell(HunterIDLE_SummonPet, me) == SpellCastResult::SPELL_CAST_OK)
    //	return false;
    if (!me->HasAura(m_UseMountID))
    {
        Pet* pPet = me->GetPet();
        if (pPet && !pPet->IsAlive())
        {
            me->StopMoving();
            TryCastSpell(HunterIDLE_RevivePet, pPet);
            return true;
        }
        else if (pPet && pPet->GetHealthPct() < 85 && !pPet->HasAura(HunterAssist_HealPet))
        {
            if (TryCastSpell(HunterAssist_HealPet, pPet) == SpellCastResult::SPELL_CAST_OK)
                return false;
        }
        else if (!pPet && TryCastSpell(HunterIDLE_SummonPet, me) == SpellCastResult::SPELL_CAST_OK)
        {
            return false;
        }
        PetAction(pPet, NULL);
    }
    if (me->HasAura(m_UseMountID))
        return false;
    if (m_BotTalentType == 1)
    {
        if (HunterIDLE_ShotAura && !me->HasAura(HunterIDLE_ShotAura) && TryCastSpell(HunterIDLE_ShotAura, me) == SpellCastResult::SPELL_CAST_OK)
            return false;
    }
    if (HunterIDLE_DragonAura)
    {
        if (!me->HasAura(HunterIDLE_DragonAura) && TryCastSpell(HunterIDLE_DragonAura, me) == SpellCastResult::SPELL_CAST_OK)
            return false;
    }
    else
    {
        if (!me->HasAura(HunterIDLE_EagleAura) && TryCastSpell(HunterIDLE_EagleAura, me) == SpellCastResult::SPELL_CAST_OK)
            return false;
    }

    return TryUpMount();
}

void BotHunterAI::ProcessMeleeSpell(Unit* pTarget)
{
    CastMeleeSpell(pTarget);
    if (me->InArena())
    {
        uint32 minLifePct = 100;
        Unit* pMinUnit = NULL;
        NearUnitVec& enemys = RangeEnemyListByHasAura(0, BOTAI_RANGESPELL_DISTANCE);
        for (Unit* pUnit : enemys)
        {
            if (pUnit == pTarget || TargetIsSuppress(pUnit))
                continue;
            if (me->GetDistance(pUnit->GetPosition()) <= 9)
                continue;
            uint32 lifePct = uint32(pUnit->GetHealthPct());
            if (!pMinUnit || lifePct < minLifePct)
            {
                pMinUnit = pUnit;
                minLifePct = lifePct;
            }
        }
        if (pMinUnit && CastRangeSpell(pMinUnit))
            return;
    }
}

void BotHunterAI::ProcessRangeSpell(Unit* pTarget)
{
    Pet* pPet = me->GetPet();
    if (!pPet && TryCastSpell(HunterIDLE_SummonPet, me) == SpellCastResult::SPELL_CAST_OK)
        return;
    PetAction(pPet, pTarget);
    if (pPet && pPet->IsAlive())
    {
        if (pPet->GetHealthPct() < 70 && !pPet->HasAura(HunterAssist_HealPet) && TryCastSpell(HunterAssist_HealPet, pPet) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    if (ProcessAura(false))
        return;

    NearUnitVec& selMeEnemys = RangeEnemyListByTargetIsMe(BOTAI_RANGESPELL_DISTANCE);
    if (selMeEnemys.size() > 0)
    {
        if (selMeEnemys.size() > 1 && TryCastSpell(HunterAssist_FalseDead, me) == SpellCastResult::SPELL_CAST_OK)
            return;
        Unit* pRndPlayer = selMeEnemys[urand(0, selMeEnemys.size() - 1)];
        if (HunterTrap_FarFrozen && pRndPlayer != pTarget && !TargetIsSuppress(pRndPlayer) && TryCastSpell(HunterTrap_FarFrozen, pRndPlayer) == SpellCastResult::SPELL_CAST_OK)
            return;
    }

    Unit* pCastEnemy = RandomRangeEnemyByCasting(BOTAI_RANGESPELL_DISTANCE);
    if (pCastEnemy)
    {
        if (m_BotTalentType == 1 && HunterShot_Silence && TryCastSpell(HunterShot_Silence, pCastEnemy) == SpellCastResult::SPELL_CAST_OK)
            return;
        else if (m_BotTalentType == 2 && HunterShot_CharmShot && TryCastSpell(HunterShot_CharmShot, pCastEnemy) == SpellCastResult::SPELL_CAST_OK)
            return;
    }

    if (!pTarget->HasAura(HunterAssist_Stamp) && TryCastSpell(HunterAssist_Stamp, pTarget) == SpellCastResult::SPELL_CAST_OK)
        return;
    if (TryCastSpell(HunterAssist_FastSpeed, me) == SpellCastResult::SPELL_CAST_OK)
        return;

    if (CastRangeSpell(pTarget))
        return;
    if (me->InArena())
    {
        uint32 minLifePct = 100;
        Unit* pMinUnit = NULL;
        Unit* pMeleeUnit = NULL;
        NearUnitVec& enemys = RangeEnemyListByHasAura(0, BOTAI_RANGESPELL_DISTANCE);
        for (Unit* pUnit : enemys)
        {
            if (pUnit == pTarget || TargetIsSuppress(pUnit))
                continue;
            float dist = me->GetDistance(pUnit->GetPosition());
            if (dist <= 9)
            {
                if (dist < 7)
                    pMeleeUnit = pUnit;
                continue;
            }
            uint32 lifePct = uint32(pUnit->GetHealthPct());
            if (!pMinUnit || lifePct < minLifePct)
            {
                pMinUnit = pUnit;
                minLifePct = lifePct;
            }
        }
        if (pMinUnit && CastRangeSpell(pMinUnit))
            return;
        if (pMeleeUnit && CastMeleeSpell(pMeleeUnit))
            return;
    }
}

bool BotHunterAI::ProcessAura(bool isFlee)
{
    if (HunterIDLE_ManaAura && CheckManaModel())
    {
        if (!me->HasAura(HunterIDLE_ManaAura) && TryCastSpell(HunterIDLE_ManaAura, me, true) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    else
    {
        if (HunterIDLE_DragonAura)
        {
            if (!me->HasAura(HunterIDLE_DragonAura) && TryCastSpell(HunterIDLE_DragonAura, me, true) == SpellCastResult::SPELL_CAST_OK)
                return true;
        }
        else
        {
            if (isFlee)
            {
                if (!me->HasAura(HunterIDLE_DodgeAura) && TryCastSpell(HunterIDLE_DodgeAura, me, true) == SpellCastResult::SPELL_CAST_OK)
                    return true;
            }
            else
            {
                if (!me->HasAura(HunterIDLE_EagleAura) && TryCastSpell(HunterIDLE_EagleAura, me, true) == SpellCastResult::SPELL_CAST_OK)
                    return true;
            }
        }
    }
    return false;
}

//bool BotHunterAI::TryStartControlCommand()
//{
//	if (m_CruxControlTarget == ObjectGuid::Empty)
//		return false;
//	if (!HunterTrap_FarFrozen)
//	{
//		m_CruxControlTarget = ObjectGuid::Empty;
//		return false;
//	}
//	Player* pTarget = ObjectAccessor::FindPlayer(m_CruxControlTarget);
//	if (!pTarget || !TargetIsNotDiminishingByType2(pTarget, DIMINISHING_DISORIENT) || !pTarget->IsAlive() || TargetIsControl(pTarget))
//	{
//		m_CruxControlTarget = ObjectGuid::Empty;
//		return false;
//	}
//	SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(HunterTrap_FarFrozen);
//	if (!spellInfo || spellInfo->IsPassive())
//	{
//		m_CruxControlTarget = ObjectGuid::Empty;
//		return false;
//	}
//	if (me->GetSpellHistory()->HasGlobalCooldown(spellInfo))
//		return true;
//	if (!me->GetSpellHistory()->IsReady(spellInfo))
//	{
//		m_CruxControlTarget = ObjectGuid::Empty;
//		return false;
//	}
//
//	if (me->IsWithinLOSInMap(pTarget) && me->GetDistance(pTarget) < BOTAI_RANGESPELL_DISTANCE)
//	{
//		TryCastSpell(HunterTrap_FarFrozen, pTarget);
//	}
//	else
//	{
//		if (!IsNotMovement())
//			m_Movement.MovementTo(m_CruxControlTarget);
//	}
//	return true;
//}
//
//float BotHunterAI::TryPushControlCommand(Player* pTarget)
//{
//	if (!pTarget || !pTarget->IsAlive() || !pTarget->IsInWorld() || me->GetMap() != pTarget->GetMap())
//	{
//		ClearCruxControlCommand();
//		return -1;
//	}
//	if (!HunterTrap_FarFrozen)
//		return -1;
//	if (!TargetIsNotDiminishingByType2(pTarget, DIMINISHING_DISORIENT))
//		return -1;
//	if (!SpellHasReady(HunterTrap_FarFrozen))
//		return -1;
//	m_CruxControlTarget = pTarget->GetGUID();
//	m_LastControlTarget = m_CruxControlTarget;
//	return me->GetDistance(pTarget->GetPosition());
//}

bool BotHunterAI::CastRangeSpell(Unit* pTarget)
{
    if (!pTarget)
        return false;
    if (me->GetDistance(pTarget->GetPosition()) <= 9)
        return false;
    NearUnitVec& targetRanges = RangeEnemyListByTargetRange(pTarget, NEEDFLEE_CHECKRANGE);
    if (targetRanges.size() > 5)
    {
        if (HunterShot_MulShot && TryCastSpell(HunterShot_MulShot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (HunterShot_AOEShot && TryCastSpell(HunterShot_AOEShot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }

    if (HunterShot_KillShot && pTarget->GetHealthPct() < 20 && TryCastSpell(HunterShot_KillShot, pTarget) == SpellCastResult::SPELL_CAST_OK)
        return true;
    if (CheckManaModel())
    {
        if (!pTarget->HasAura(HunterDebug_Mana, me->GetGUID()) && !pTarget->HasAura(HunterDebug_Damage, me->GetGUID()) && TryCastSpell(HunterDebug_Mana, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    else
    {
        if (!pTarget->HasAura(HunterDebug_Mana, me->GetGUID()) && !pTarget->HasAura(HunterDebug_Damage, me->GetGUID()) && TryCastSpell(HunterDebug_Damage, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    if (me->InArena() && TryCastSpell(HunterShot_MulShot, pTarget) == SpellCastResult::SPELL_CAST_OK)
        return true;
    if (TryCastSpell(HunterAssist_FastSpeed, me) == SpellCastResult::SPELL_CAST_OK)
        return true;
    if (TargetIsMelee(pTarget->ToPlayer()) && !pTarget->HasAura(HunterMelee_DecSpeed) && !pTarget->HasAura(HunterShot_Shock) && !TargetIsSuppress(pTarget))
    {
        if (TryCastSpell(HunterShot_Shock, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    if (pTarget->GetTarget() == me->GetGUID())
    {
        if (m_BotTalentType == 1 && HunterShot_Aim && TryCastSpell(HunterShot_Aim, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (m_BotTalentType == 1 && HunterShot_QMLShot && me->GetLevel() == 80 && TryCastSpell(HunterShot_QMLShot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (m_BotTalentType == 2 && HunterShot_Explode && TryCastSpell(HunterShot_Explode, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (TryCastSpell(HunterShot_MgcShot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (m_BotTalentType == 2 && HunterTrap_Shot && TryCastSpell(HunterTrap_Shot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    else
    {
        if (m_BotTalentType == 1 && HunterShot_Aim && TryCastSpell(HunterShot_Aim, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (m_BotTalentType == 1 && HunterShot_QMLShot && me->GetLevel() == 80 && TryCastSpell(HunterShot_QMLShot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (m_BotTalentType == 2 && HunterShot_Explode && TryCastSpell(HunterShot_Explode, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (TryCastSpell(HunterShot_MgcShot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (m_BotTalentType == 2 && HunterTrap_Shot && TryCastSpell(HunterTrap_Shot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (TryCastSpell(HunterShot_Cast, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    return false;
}

bool BotHunterAI::CastMeleeSpell(Unit* pTarget)
{
    if (!pTarget)
        return false;
    if (m_BotTalentType == 2 && HunterDebug_Sleep && TargetIsMelee(pTarget->ToPlayer()) && me->GetDistance(pTarget->GetPosition()) < 12)
    {
        if (!TargetIsSuppress(pTarget) && TryCastSpell(HunterDebug_Sleep, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    if (TargetIsMelee(pTarget->ToPlayer()) && !pTarget->HasAura(HunterMelee_DecSpeed) && !pTarget->HasAura(HunterShot_Shock) && !TargetIsSuppress(pTarget))
    {
        if (TryCastSpell(HunterMelee_DecSpeed, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
        if (TryCastSpell(HunterShot_Shock, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    if (!me->HasAura(HunterMelee_NextAtt))
        TryCastSpell(HunterMelee_NextAtt, pTarget);
    if (TryCastSpell(HunterMelee_MeleeAtt, pTarget) == SpellCastResult::SPELL_CAST_OK)
        return true;
    if (m_BotTalentType == 2 && HunterMelee_BackRoot)
    {
        if (TryCastSpell(HunterMelee_BackRoot, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    return false;
}

void BotHunterAI::PetAction(Pet* pPet, Unit* pTarget)
{
    if (!pPet)
        return;
    if (pPet->GetVictim() == pTarget)
        return;
    WorldSession* pSession = me->GetSession();
    if (pTarget)
        pSession->HandlePetActionHelper(pPet, pPet->GetGUID(), 2, 7, pTarget->GetGUID());
    else
        pSession->HandlePetActionHelper(pPet, pPet->GetGUID(), 1, 7, ObjectGuid::Empty);
}

bool BotHunterAI::HasRootMechanic()
{
    if (HasAuraMechanic(me, Mechanics::MECHANIC_CHARM) ||
        HasAuraMechanic(me, Mechanics::MECHANIC_FEAR) ||
        HasAuraMechanic(me, Mechanics::MECHANIC_ROOT) ||
        HasAuraMechanic(me, Mechanics::MECHANIC_SLEEP) ||
        HasAuraMechanic(me, Mechanics::MECHANIC_POLYMORPH) ||
        HasAuraMechanic(me, Mechanics::MECHANIC_HORROR) ||
        HasAuraMechanic(me, Mechanics::MECHANIC_STUN))
        return true;
    return false;
}

bool BotHunterAI::TargetIsSuppress(Unit* pTarget)
{
    if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_CHARM) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_DISORIENTED) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_DISTRACT) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_SLEEP) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_POLYMORPH) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_BANISH) ||
        HasAuraMechanic(pTarget, Mechanics::MECHANIC_IMMUNE_SHIELD))
        return true;
    return false;
}

bool BotHunterAI::CheckManaModel()
{
    uint32 manaPct = GetManaPowerPer();
    if (m_IsReviveManaModel)
    {
        if (manaPct > 70)
            m_IsReviveManaModel = false;
    }
    else
    {
        if (manaPct < 8)
            m_IsReviveManaModel = true;
    }
    return m_IsReviveManaModel;
}
