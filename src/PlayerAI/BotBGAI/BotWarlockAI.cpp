#include "BotWarlockAI.h"
#include "Pet.h"
#include "Group.h"
#include "SpellHistory.h"
#include "WorldSession.h"
#include "Item.h"
#include "Bag.h"

void BotWarlockAI::InitializeSpells()
{
    WarlockIDLE_LowArmor = FindMaxRankSpellByExist(696);
    WarlockIDLE_Armor = FindMaxRankSpellByExist(47889);
    WarlockIDLE_HighArmor = FindMaxRankSpellByExist(47893);
    WarlockIDLE_SoulLink = FindMaxRankSpellByExist(19028);
    WarlockIDLE_ShadowShield = FindMaxRankSpellByExist(47891);
    WarlockIDLE_SummonFireDemon = FindMaxRankSpellByExist(688);
    WarlockIDLE_SummonHollowDemon = FindMaxRankSpellByExist(697);
    WarlockIDLE_SummonSuccubus = FindMaxRankSpellByExist(712);
    WarlockIDLE_SummonDogDemon = FindMaxRankSpellByExist(691);
    WarlockIDLE_SummonGuardDemon = FindMaxRankSpellByExist(30146);
    WarlockIDLE_FastSummon = FindMaxRankSpellByExist(18708);
    WarlockIDLE_OpenGate = FindMaxRankSpellByExist(48018);
    WarlockIDLE_TeleGate = FindMaxRankSpellByExist(48020);
    WarlockIDLE_SummonRite = FindMaxRankSpellByExist(29893);

    WarlockDemon_ToDemon = FindMaxRankSpellByExist(47241);
    WarlockDemon_Charge = FindMaxRankSpellByExist(54785);
    WarlockDemon_MeleeAOE = FindMaxRankSpellByExist(50581);
    WarlockDemon_Sacrifice = FindMaxRankSpellByExist(50589);

    WarlockAssist_DemonPower = FindMaxRankSpellByExist(47193);
    WarlockAssist_ExtractMana = FindMaxRankSpellByExist(59092);
    WarlockAssist_ConvertMana = FindMaxRankSpellByExist(57946);
    WarlockAssist_StealLife = FindMaxRankSpellByExist(47857);
    WarlockAssist_StealMana = FindMaxRankSpellByExist(5138);
    WarlockAssist_BaseFear = FindMaxRankSpellByExist(6215);
    WarlockAssist_FastFear = FindMaxRankSpellByExist(47860);
    WarlockAssist_AOEFear = FindMaxRankSpellByExist(17928);

    WarlockAOE_MeleeFire = FindMaxRankSpellByExist(61290);
    WarlockAOE_RainFire = FindMaxRankSpellByExist(47820);
    WarlockAOE_ShadowRage = FindMaxRankSpellByExist(47847);

    WarlockCurse_UpDmg = FindMaxRankSpellByExist(47865);
    WarlockCurse_MoveLow = FindMaxRankSpellByExist(18223);
    WarlockCurse_MgcDmg = FindMaxRankSpellByExist(47864);
    WarlockCurse_MeleeLow = FindMaxRankSpellByExist(50511);
    WarlockCurse_CastLow = FindMaxRankSpellByExist(11719);

    WarlockDot_LeechSoul = FindMaxRankSpellByExist(59164);
    WarlockDot_HighDmg = FindMaxRankSpellByExist(47843);
    WarlockDot_LowDmg = FindMaxRankSpellByExist(47813);
    WarlockDot_AOEDmg = FindMaxRankSpellByExist(47836);
    WarlockDot_Sacrifice = FindMaxRankSpellByExist(47811);

    WarlockCast_ShadowArrow = FindMaxRankSpellByExist(47809);
    WarlockCast_ShadowShock = FindMaxRankSpellByExist(47827);
    WarlockCast_ChaosArrow = FindMaxRankSpellByExist(59172);
    WarlockCast_FullBurn = FindMaxRankSpellByExist(47838);
    WarlockCast_FireBurn = FindMaxRankSpellByExist(17962);
    WarlockCast_BigFireBall = FindMaxRankSpellByExist(47825);

    WarlockFlag_SoulItem = 6265;
    WarlockFlag_SoulLink = 25228;
    WarlockFlag_OpenGate = 48018;
    WarlockFlag_Sacrifice = 50589;
}

void BotWarlockAI::InitializePetSpells()
{
    if (m_IsInitedPetSpells)
        return;
    Pet* pPet = me->GetPet();
    if (!pPet || !pPet->IsAlive())
        return;

    WarlockPetDemon_Charge = FindPetMaxRankSpellByExist(47996);
    WarlockPetDemon_Melee = FindPetMaxRankSpellByExist(47994);
    WarlockPetDog_MagicBuf = FindPetMaxRankSpellByExist(57567);
    WarlockPetDog_EatMgc = FindPetMaxRankSpellByExist(48011);
    WarlockPetDog_Bite = FindPetMaxRankSpellByExist(54053);
    WarlockPetDog_Silence = FindPetMaxRankSpellByExist(19647);
    WarlockPetSuccubus_Lash = FindPetMaxRankSpellByExist(47992);
    WarlockPetSuccubus_Sneak = FindPetMaxRankSpellByExist(7870);
    WarlockPetSuccubus_Charm = FindPetMaxRankSpellByExist(6358);
    WarlockPetGhost_Shield = FindPetMaxRankSpellByExist(47983);
    WarlockPetGhost_Sneak = FindPetMaxRankSpellByExist(4511);
    WarlockPetGhost_Stamp = FindPetMaxRankSpellByExist(47982);
    WarlockPetGhost_FireArrow = FindPetMaxRankSpellByExist(47964);

    SettingPetAutoCastSpell(WarlockPetDemon_Charge, true);
    SettingPetAutoCastSpell(WarlockPetDemon_Melee, true);
    SettingPetAutoCastSpell(WarlockPetDog_MagicBuf, false);
    SettingPetAutoCastSpell(WarlockPetDog_EatMgc, false);//true
    SettingPetAutoCastSpell(WarlockPetDog_Bite, true);
    SettingPetAutoCastSpell(WarlockPetDog_Silence, false);
    SettingPetAutoCastSpell(WarlockPetSuccubus_Lash, true);
    SettingPetAutoCastSpell(WarlockPetSuccubus_Sneak, true);
    SettingPetAutoCastSpell(WarlockPetSuccubus_Charm, false);
    SettingPetAutoCastSpell(WarlockPetGhost_Shield, true);
    SettingPetAutoCastSpell(WarlockPetGhost_Sneak, true);
    SettingPetAutoCastSpell(WarlockPetGhost_Stamp, true);
    SettingPetAutoCastSpell(WarlockPetGhost_FireArrow, true);

    m_IsInitedPetSpells = true;
}

void BotWarlockAI::UpdateTalentType()
{
    m_BotTalentType = FindTalentType(me);// PlayerBotSetting::FindPlayerTalentType(me);
}

void BotWarlockAI::ResetBotAI()
{
    BotBGAI::ResetBotAI();
    m_IsInitedPetSpells = false;
    m_IsSummonPeted = false;
    UpdateTalentType();
    InitializeSpells();
    m_LastCharmTarget = ObjectGuid::Empty;
    m_LastCharmTick = 0;
}

uint32 BotWarlockAI::GetManaPowerPer()
{
    float per = (float)me->GetPower(POWER_MANA) / (float)me->GetMaxPower(POWER_MANA);
    return (uint32)(per * 100);
}

void BotWarlockAI::EachTick()
{
    BotBGAI::EachTick();
    InsureSoulItem();
    if (m_LastCharmTick > 0)
    {
        if (m_LastCharmTick + 6000 <= getMSTime())
        {
            m_LastCharmTarget = ObjectGuid::Empty;
            m_LastCharmTick = 0;
        }
    }
    if (!me->IsInCombat() || !me->InArena())
        return;
    Group* pGroup = me->GetGroup();
    if (pGroup)
    {
        Group::MemberSlotList const& memList = pGroup->GetMemberSlots();
        for (Group::MemberSlot const& slot : memList)
        {
            Player* player = ObjectAccessor::FindPlayer(slot.guid);
            if (!player || me->GetMap() != player->GetMap() || !player->IsInWorld() || player->IsPlayerBot())
                continue;
            uint32 max = me->GetMaxPower(Powers::POWER_MANA);
            uint32 power = me->GetPower(Powers::POWER_MANA);
            me->SetPower(Powers::POWER_MANA, (max / 650) + power);
            return;
        }
    }
    else
        return;
    uint32 max = me->GetMaxPower(Powers::POWER_MANA);
    uint32 power = me->GetPower(Powers::POWER_MANA);
    me->SetPower(Powers::POWER_MANA, (max / 450) + power);
}

bool BotWarlockAI::NeedFlee()
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

void BotWarlockAI::ProcessReady()
{
    ProcessNormalSpell();
}

void BotWarlockAI::ProcessFlee()
{
    if (!me->HasUnitState(UNIT_STATE_CASTING))
        FleeMovement();
    if (ProcessMana(true))
        return;

    NearUnitVec& enemys = RangeEnemyListByTargetIsMe(NEEDFLEE_CHECKRANGE);
    if (enemys.empty())
        return;
    Unit* pRndPlayer = NULL;
    for (Unit* player : enemys)
    {
        if (!TargetIsFear(player))
        {
            pRndPlayer = player;
            break;
        }
    }
    if (pRndPlayer == NULL)
        pRndPlayer = enemys[urand(0, enemys.size() - 1)];
    if (ProcessGate(pRndPlayer, false))
        return;
    Pet* pPet = me->GetPet();
    if (pPet && !pPet->HasUnitState(UNIT_STATE_CASTING))
    {
        Unit* pPetTarget = pPet->GetVictim();
        bool isValidTarget = false;
        for (Unit* player : enemys)
        {
            if (pPetTarget == player)
            {
                isValidTarget = true;
                break;
            }
        }
        if (!isValidTarget)
        {
            PetAction(pPet, pRndPlayer);
        }
    }

    if (enemys.size() <= 2 && !TargetIsFear(pRndPlayer))
    {
        if (enemys.size() > 1)
        {
            if (WarlockAssist_AOEFear && TargetIsNotDiminishingByType2(pRndPlayer, DiminishingGroup::DIMINISHING_FEAR))
            {
                me->StopMoving();
                if (TryCastSpell(WarlockAssist_AOEFear, pRndPlayer) == SpellCastResult::SPELL_CAST_OK)
                    return;
            }
        }
        if (WarlockAssist_FastFear && TryCastSpell(WarlockAssist_FastFear, pRndPlayer) == SpellCastResult::SPELL_CAST_OK)
        {
            return;
        }
        if (WarlockAssist_BaseFear && !m_NeedReserveCtrlSpell)
        {
            if (!BotUtility::ControllSpellDiminishing || TargetIsNotDiminishingByType2(pRndPlayer, DiminishingGroup::DIMINISHING_FEAR))
            {
                me->StopMoving();
                if (TryCastSpell(WarlockAssist_BaseFear, pRndPlayer) == SpellCastResult::SPELL_CAST_OK)
                    return;
            }
        }
    }
    ProcessPetSpell(pRndPlayer, true);

    if (m_BotTalentType == 1 && WarlockAssist_DemonPower && TryCastSpell(WarlockAssist_DemonPower, me) == SpellCastResult::SPELL_CAST_OK)
        return;
    if (WarlockAOE_MeleeFire && TryCastSpell(WarlockAOE_MeleeFire, pRndPlayer) == SpellCastResult::SPELL_CAST_OK)
        return;
    if (m_BotTalentType == 2 && enemys.size() > 1 && WarlockAOE_ShadowRage)
    {
        if (TryCastSpell(WarlockAOE_ShadowRage, pRndPlayer) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    if (WarlockCurse_MoveLow && m_BotTalentType == 0 && !pRndPlayer->HasAura(WarlockCurse_MoveLow, me->GetGUID()) && TryCastSpell(WarlockCurse_MoveLow, pRndPlayer) == SpellCastResult::SPELL_CAST_OK)
        return;

    if (m_BotTalentType == 2 && WarlockCast_ShadowShock)
    {
        if (!pRndPlayer->HasAura(WarlockCast_ShadowShock, me->GetGUID()) && TryCastSpell(WarlockCast_ShadowShock, pRndPlayer, true) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    ProcessDot(pRndPlayer, false);
}

void BotWarlockAI::ProcessPetSpell(Unit* pTarget, bool fleeing)
{
    Pet* pPet = me->GetPet();
    if (!pPet || pPet->HasUnitState(UNIT_STATE_CASTING))
        return;

    Unit* pPetTarget = RandomRangeEnemyByCasting(BOTAI_RANGESPELL_DISTANCE);
    if (pPetTarget && WarlockPetDog_Silence)
    {
        if (PetTryCastSpell(WarlockPetDog_Silence, pPetTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }

    if (WarlockPetSuccubus_Charm)
    {
        std::list<Unit*> rangePlayers;
        NearUnitVec enemys;
        if (fleeing)
            enemys = RangeEnemyListByTargetIsMe(BOTAI_RANGESPELL_DISTANCE);
        else
            enemys = RangeEnemyListByHasAura(0, BOTAI_RANGESPELL_DISTANCE);
        for (Unit* pEnemy : enemys)
        {
            if (!fleeing && pEnemy == pTarget)
                continue;
            if (TargetIsFear(pEnemy))
                continue;
            if (SearchFriendTargetIsTarget(pEnemy, BOTAI_RANGESPELL_DISTANCE).size() > 0)
                continue;
            if (!TargetIsMelee(pEnemy->ToPlayer()))
            {
                rangePlayers.push_back(pEnemy);
                continue;
            }
            if (TargetIsNotDiminishingByType2(pEnemy, DiminishingGroup::DIMINISHING_FEAR))
            {
                if (PetTryCastSpell(WarlockPetSuccubus_Charm, pEnemy) == SpellCastResult::SPELL_CAST_OK)
                {
                    m_LastCharmTarget = pEnemy->GetGUID();
                    m_LastCharmTick = getMSTime();
                    return;
                }
            }
        }
        if (rangePlayers.size() > 0)
        {
            Unit* pEnemy = (*rangePlayers.begin());
            if (TargetIsNotDiminishingByType2(pEnemy, DiminishingGroup::DIMINISHING_FEAR))
            {
                if (PetTryCastSpell(WarlockPetSuccubus_Charm, pEnemy) == SpellCastResult::SPELL_CAST_OK)
                {
                    m_LastCharmTarget = pEnemy->GetGUID();
                    m_LastCharmTick = getMSTime();
                    return;
                }
            }
        }
    }
}

bool BotWarlockAI::CastingControlCommandSpell()
{
    if (m_LastControlTarget == ObjectGuid::Empty || !WarlockAssist_BaseFear)
        return false;
    if (!me->HasUnitState(UNIT_STATE_CASTING))
        return false;
    Spell* pCurrentSpell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_GENERIC_SPELL);
    if (pCurrentSpell)
    {
        if (pCurrentSpell->m_spellInfo && pCurrentSpell->m_spellInfo->Id == WarlockAssist_BaseFear)
        {
            return pCurrentSpell->m_targets.GetUnitTargetGUID() == m_LastControlTarget;
        }
    }
    if (!pCurrentSpell)
    {
        pCurrentSpell = me->GetCurrentSpell(CurrentSpellTypes::CURRENT_CHANNELED_SPELL);
        if (pCurrentSpell && pCurrentSpell->m_spellInfo && pCurrentSpell->m_spellInfo->Id == WarlockAssist_BaseFear)
        {
            return pCurrentSpell->m_targets.GetUnitTargetGUID() == m_LastControlTarget;
        }
    }
    return false;
}

bool BotWarlockAI::TryStartControlCommand()
{
    if (m_CruxControlTarget == ObjectGuid::Empty)
        return false;
    if (!WarlockAssist_BaseFear)
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
    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(WarlockAssist_BaseFear);
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

    if (me->IsWithinLOSInMap(pTarget) && me->GetDistance(pTarget) < BOTAI_RANGESPELL_DISTANCE)
    {
        m_Movement.ClearMovement();
        TryCastSpell(WarlockAssist_BaseFear, pTarget);
    }
    else
    {
        if (!IsNotMovement())
            m_Movement.MovementTo(m_CruxControlTarget);
    }
    return true;
}

float BotWarlockAI::TryPushControlCommand(Player* pTarget)
{
    if (!pTarget || !pTarget->IsAlive() || !pTarget->IsInWorld() || me->GetMap() != pTarget->GetMap())
    {
        ClearCruxControlCommand();
        return -1;
    }
    if (!WarlockAssist_BaseFear)
        return -1;
    if (!TargetIsNotDiminishingByType2(pTarget, DIMINISHING_FEAR))
        return -1;
    if (!SpellHasReady(WarlockAssist_BaseFear))
        return -1;
    m_CruxControlTarget = pTarget->GetGUID();
    m_LastControlTarget = m_CruxControlTarget;
    return me->GetDistance(pTarget->GetPosition());
}

bool BotWarlockAI::ProcessGate(Unit* pTarget, bool onlyBuild)
{
    if (!pTarget || !WarlockIDLE_OpenGate || !WarlockIDLE_TeleGate || !me->InArena())
        return false;
    uint32 manaPct = GetManaPowerPer();
    if (manaPct < 15)
        return false;
    float distGate = me->GetDistance(m_GatePos);
    NearUnitVec& enemys = RangeEnemyListByHasAura(0, NEEDFLEE_CHECKRANGE);
    if (me->HasAura(WarlockFlag_OpenGate) && distGate > 8 && distGate < 30 && m_GatePos.GetPositionZ() != 0)
    {
        if (enemys.size() > 1)
        {
            if (TryCastSpell(WarlockIDLE_TeleGate, me) == SpellCastResult::SPELL_CAST_OK)
            {
                OnCastTeleGate();
                return true;
            }
        }
    }
    else if (enemys.empty() && me->GetDistance(pTarget->GetPosition()) < BOTAI_RANGESPELL_DISTANCE && distGate > 25)
    {
        m_Movement.ClearMovement();
        if (onlyBuild && TryCastSpell(WarlockIDLE_OpenGate, me, true) == SpellCastResult::SPELL_CAST_OK)
        {
            m_GatePos = me->GetPosition();
            return true;
        }
    }
    else if (!me->HasAura(WarlockFlag_OpenGate))
    {
        m_GatePos = Position();
    }
    return false;
}

bool BotWarlockAI::ProcessMana(bool isMoveing)
{
    uint32 manaPct = GetManaPowerPer();
    if (manaPct > 30)
        return false;
    if (m_BotTalentType == 0 && WarlockAssist_ExtractMana)
    {
        if (TryCastSpell(WarlockAssist_ExtractMana, me) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    if (!isMoveing && me->InArena())
    {
        if (Unit* pTarget = me->GetSelectedUnit())
        {
            switch (pTarget->GetClass())
            {
            case CLASS_WARRIOR:
            case CLASS_ROGUE:
            case CLASS_DEATH_KNIGHT:
            case CLASS_HUNTER:
                break;
            case CLASS_MAGE:
            case CLASS_WARLOCK:
            case CLASS_PALADIN:
            case CLASS_PRIEST:
            case CLASS_SHAMAN:
            case CLASS_DRUID:
                if (TargetManaPCT(pTarget) > 8)
                {
                    if (WarlockAssist_StealMana && pTarget->IsAlive() && TryCastSpell(WarlockAssist_StealMana, pTarget) == SpellCastResult::SPELL_CAST_OK)
                        return true;
                }
            }
        }
    }
    if (me->GetHealthPct() > 65)
    {
        if (TryCastSpell(WarlockAssist_ConvertMana, me) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }
    return false;
}

bool BotWarlockAI::ProcessDot(Unit* pTarget, bool canCastTime)
{
    if (!pTarget)
        return false;
    if (!WarlockDot_AOEDmg && !pTarget->HasAura(WarlockDot_LowDmg, me->GetGUID()) && TryCastSpell(WarlockDot_LowDmg, pTarget) == SpellCastResult::SPELL_CAST_OK)
        return true;
    if (canCastTime)
    {
        //if (WarlockDot_AOEDmg && urand(0, 99) < 20)
        //{
        //	if (!pTarget->HasAura(WarlockDot_AOEDmg, me->GetGUID()) && TryCastSpell(WarlockDot_AOEDmg, pTarget) == SpellCastResult::SPELL_CAST_OK)
        //		return true;
        //}
        if (m_BotTalentType == 0)
        {
            if (!pTarget->HasAura(WarlockDot_LeechSoul, me->GetGUID()) && TryCastSpell(WarlockDot_LeechSoul, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return true;
            if (!pTarget->HasAura(WarlockDot_HighDmg, me->GetGUID()) && TryCastSpell(WarlockDot_HighDmg, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return true;
        }
        else if (!pTarget->HasAura(WarlockDot_Sacrifice, me->GetGUID()) && TryCastSpell(WarlockDot_Sacrifice, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }

    if (m_BotTalentType == 0 && me->InArena() && WarlockCurse_MoveLow)
    {
        if (TargetIsMelee(pTarget->ToPlayer()) && !pTarget->HasAura(WarlockCurse_MoveLow, me->GetGUID()) && !TargetIsFear(pTarget))
        {
            if (TryCastSpell(WarlockCurse_MoveLow, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return true;
        }
        NearUnitVec& enemys = RangeEnemyListByHasAura(0, BOTAI_RANGESPELL_DISTANCE);
        for (Unit* pUnit : enemys)
        {
            if (TargetIsMelee(pUnit->ToPlayer()) && !pUnit->HasAura(WarlockCurse_MoveLow, me->GetGUID()) && !TargetIsFear(pTarget))
            {
                if (TryCastSpell(WarlockCurse_MoveLow, pUnit) == SpellCastResult::SPELL_CAST_OK)
                    return true;
            }
        }
    }

    if (pTarget->HasAura(WarlockCurse_MoveLow, me->GetGUID()))
        return false;
    if (pTarget->GetTarget() == me->GetGUID())
    {
        Player* pPlayer = (Player*)pTarget->ToPlayer();
        if (TargetIsCastMagic(pPlayer))
        {
            if (!pTarget->HasAura(WarlockCurse_CastLow, me->GetGUID()) && TryCastSpell(WarlockCurse_CastLow, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return true;
        }
        else
        {
            if (!pTarget->HasAura(WarlockCurse_MeleeLow, me->GetGUID()) && TryCastSpell(WarlockCurse_MeleeLow, pTarget) == SpellCastResult::SPELL_CAST_OK)
                return true;
        }
    }
    else
    {
        if (!pTarget->HasAura(WarlockCurse_MgcDmg, me->GetGUID()) && TryCastSpell(WarlockCurse_MgcDmg, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return true;
    }

    return false;
}

bool BotWarlockAI::ProcessFear(Unit* pTarget)
{
    if (!pTarget || !WarlockAssist_BaseFear)
        return false;
    if (me->InArena())
    {
        NearUnitVec& targetMeEnemys = RangeEnemyListByHasAura(0, NEEDFLEE_CHECKRANGE);
        if (WarlockAssist_AOEFear && targetMeEnemys.size() > 2)
        {
            me->StopMoving();
            if (TryCastSpell(WarlockAssist_AOEFear, targetMeEnemys[urand(0, targetMeEnemys.size() - 1)]) == SpellCastResult::SPELL_CAST_OK)
                return true;
        }
    }
    if (m_NeedReserveCtrlSpell)
        return false;
    if (TargetIsFear(pTarget))
    {
        NearUnitVec& enemys = RangeEnemyListByHasAura(0, BOTAI_SEARCH_RANGE);
        for (Unit* pUnit : enemys)
        {
            if (TargetIsFear(pUnit))
                continue;
            //if (m_LastCharmTarget == pUnit->GetGUID())
            //	continue;
            if (BotUtility::ControllSpellDiminishing && !TargetIsNotDiminishingByType2(pUnit, DiminishingGroup::DIMINISHING_FEAR))
                continue;
            me->StopMoving();
            if (TryCastSpell(WarlockAssist_BaseFear, pUnit) == SpellCastResult::SPELL_CAST_OK)
            {
                return true;
            }
        }
        return false;
    }

    if (BotUtility::ControllSpellDiminishing && !TargetIsNotDiminishingByType2(pTarget, DiminishingGroup::DIMINISHING_FEAR))
    {
        return false;
    }
    //if (m_LastCharmTarget == pTarget->GetGUID())
    //	return false;

    me->StopMoving();
    if (TryCastSpell(WarlockAssist_BaseFear, pTarget) == SpellCastResult::SPELL_CAST_OK)
    {
        return true;
    }
    return false;
}

bool BotWarlockAI::ProcessNormalSpell()
{
    if (me->HasUnitState(UNIT_STATE_CASTING) || !me->InBattleground() || !me->IsInWorld())
        return true;
    InitializePetSpells();

    if (WarlockPetDog_MagicBuf && !me->HasAura(WarlockPetDog_MagicBuf))
        PetTryCastSpell(WarlockPetDog_MagicBuf, me);
    if (WarlockIDLE_HighArmor)
    {
        if (!me->HasAura(WarlockIDLE_HighArmor) && TryCastSpell(WarlockIDLE_HighArmor, me) == SpellCastResult::SPELL_CAST_OK)
            return false;
    }
    else if (WarlockIDLE_Armor)
    {
        if (!me->HasAura(WarlockIDLE_Armor) && TryCastSpell(WarlockIDLE_Armor, me) == SpellCastResult::SPELL_CAST_OK)
            return false;
    }
    else if (WarlockIDLE_LowArmor)
    {
        if (!me->HasAura(WarlockIDLE_LowArmor) && TryCastSpell(WarlockIDLE_LowArmor, me) == SpellCastResult::SPELL_CAST_OK)
            return false;
    }
    if (WarlockIDLE_ShadowShield)
    {
        if (!me->HasAura(WarlockIDLE_ShadowShield) && TryCastSpell(WarlockIDLE_ShadowShield, me) == SpellCastResult::SPELL_CAST_OK)
            return false;
    }
    if (!me->HasAura(m_UseMountID))
    {
        if (!m_IsSummonPeted || me->GetPetGUID().IsEmpty())
        {
            if (WarlockIDLE_FastSummon)
                TryCastSpell(WarlockIDLE_FastSummon, me);
            if (m_BotTalentType == 1 && WarlockIDLE_SummonGuardDemon)
            {
                if (TryCastSpell(WarlockIDLE_SummonGuardDemon, me) == SpellCastResult::SPELL_CAST_OK)
                {
                    m_IsInitedPetSpells = false;
                    m_IsSummonPeted = true;
                    return true;
                }
            }
            else
            {
                uint32 rndRate = urand(0, 99);
                if (me->InArena())
                {
                    if (TryCastSpell(WarlockIDLE_SummonSuccubus, me) == SpellCastResult::SPELL_CAST_OK)
                    {
                        m_IsInitedPetSpells = false;
                        m_IsSummonPeted = true;
                        return true;
                    }
                }
                else if (rndRate < 45)
                {
                    if (TryCastSpell(WarlockIDLE_SummonSuccubus, me) == SpellCastResult::SPELL_CAST_OK)
                    {
                        m_IsInitedPetSpells = false;
                        m_IsSummonPeted = true;
                        return true;
                    }
                }
                else if (rndRate < 90)
                {
                    if (TryCastSpell(WarlockIDLE_SummonDogDemon, me) == SpellCastResult::SPELL_CAST_OK)
                    {
                        m_IsInitedPetSpells = false;
                        m_IsSummonPeted = true;
                        return true;
                    }
                }
                else
                {
                    if (TryCastSpell(WarlockIDLE_SummonFireDemon, me) == SpellCastResult::SPELL_CAST_OK)
                    {
                        m_IsInitedPetSpells = false;
                        m_IsSummonPeted = true;
                        return true;
                    }
                }
            }
        }
        else if (m_BotTalentType == 1 && WarlockIDLE_SoulLink)
        {
            if (!me->HasAura(WarlockFlag_SoulLink) && TryCastSpell(WarlockIDLE_SoulLink, me) == SpellCastResult::SPELL_CAST_OK)
                return false;
        }
    }
    return TryUpMount();
}

void BotWarlockAI::ProcessMeleeSpell(Unit* pTarget)
{
    if (ProcessGate(pTarget, false))
        return;
}

void BotWarlockAI::ProcessRangeSpell(Unit* pTarget)
{
    PetAction(me->GetPet(), pTarget);
    if (ProcessMana(false))
        return;
    if (ProcessGate(pTarget, true))
        return;

    NearUnitVec& enemys = RangeEnemyListByTargetRange(pTarget, NEEDFLEE_CHECKRANGE);
    if (enemys.size() > 1)
    {
        if (m_BotTalentType == 2 && TryCastSpell(WarlockAOE_ShadowRage, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (WarlockDot_AOEDmg && !pTarget->HasAura(WarlockDot_AOEDmg, me->GetGUID()))
        {
            uint32 curTick = getMSTime();
            if (curTick > m_LastAOEDotTick && TryCastSpell(WarlockDot_AOEDmg, pTarget) == SpellCastResult::SPELL_CAST_OK)
            {
                m_LastAOEDotTick = curTick + 8000;
                return;
            }
        }
        if (enemys.size() > 5 && TryCastSpell(WarlockAOE_RainFire, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    ProcessPetSpell(pTarget, false);
    if (ProcessFear(pTarget))
        return;
    if (ProcessDot(pTarget, true))
        return;

    if (m_BotTalentType == 0 && WarlockAssist_StealLife)
    {
        if (TryCastSpell(WarlockAssist_StealLife, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    if (m_BotTalentType == 2 && WarlockCast_ChaosArrow && TryCastSpell(WarlockCast_ChaosArrow, pTarget) == SpellCastResult::SPELL_CAST_OK)
        return;
    if (m_BotTalentType == 2 && pTarget->HasAura(WarlockDot_Sacrifice))
    {
        if (WarlockCast_FullBurn && TryCastSpell(WarlockCast_FullBurn, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
        if (WarlockCast_FireBurn && TryCastSpell(WarlockCast_FireBurn, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }

    if (pTarget->GetHealthPct() < 65)
    {
        if (TryCastSpell(WarlockCast_ShadowArrow, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
    else
    {
        if (TryCastSpell(WarlockCast_BigFireBall, pTarget) == SpellCastResult::SPELL_CAST_OK)
            return;
    }
}

void BotWarlockAI::PetAction(Pet* pPet, Unit* pTarget)
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

bool BotWarlockAI::TargetIsFear(Unit* pTarget)
{
    if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_CHARM))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_DISORIENTED))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_FEAR))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_SLEEP))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_STUN))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_FREEZE))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_KNOCKOUT))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_POLYMORPH))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_BANISH))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_HORROR))
        return true;
    else if (HasAuraMechanic(pTarget, Mechanics::MECHANIC_SAPPED))
        return true;
    return false;
}

void BotWarlockAI::OnCastTeleGate()
{
    if (m_GatePos.GetPositionZ() == 0)
        return;
    me->GetMotionMaster()->Clear();
    m_Movement.ClearMovement();
    SetTeleport(m_GatePos);
    //me->UpdatePosition(m_GatePos, true);
    //m_Movement.SyncPosition(m_GatePos, true);
}

void BotWarlockAI::InsureSoulItem()
{
    for (uint8 slot = InventoryPackSlots::INVENTORY_SLOT_ITEM_START; slot < InventoryPackSlots::INVENTORY_SLOT_ITEM_END; slot++)
    {
        Item* pItem = me->GetItemByPos(255, slot);
        if (!pItem)
            continue;
        const ItemTemplate* pTemplate = pItem->GetTemplate();
        if (!pTemplate)
            continue;
        if (pTemplate->ItemId == WarlockFlag_SoulItem)
            return;
    }
    for (uint8 i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
    {
        if (Bag* pBag = me->GetBagByPos(i))
        {
            for (uint32 j = 0; j < pBag->GetBagSize(); j++)
            {
                Item* pItem = pBag->GetItemByPos(uint8(j));
                if (!pItem)
                    continue;
                const ItemTemplate* pTemplate = pItem->GetTemplate();
                if (!pTemplate)
                    continue;
                if (pTemplate->ItemId == WarlockFlag_SoulItem)
                    return;
            }
        }
    }

    ItemTemplate const* pTemplate = sObjectMgr->GetItemTemplate(WarlockFlag_SoulItem);
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
    me->StoreNewItem(dest, pTemplate->ItemId, true, GenerateItemRandomPropertyId(pTemplate->ItemId));
}

uint32 BotWarlockAI::TryCastSummonRiteSpell()
{
    if (WarlockIDLE_SummonRite == 0 || !me->HasSpell(WarlockIDLE_SummonRite))
        return 0;
    if (!me->IsAlive() || me->IsInCombat() || me->HasUnitState(UNIT_STATE_CASTING))
        return 0;
    std::list<GameObject*> goResults;
    Trinity::GameObjectInRangeCheck checkerGO(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), BOTAI_SEARCH_RANGE);
    Trinity::GameObjectListSearcher<Trinity::GameObjectInRangeCheck> searcherGO(me, goResults, checkerGO);
    //me->VisitNearbyGridObject(BOTAI_SEARCH_RANGE, searcherGO);
    Cell::VisitGridObjects(me, searcherGO, BOTAI_SEARCH_RANGE);
    for (GameObject* go : goResults)
    {
        if (go->GetSpellId() == WarlockIDLE_SummonRite)
            return 0;
    }
    Group* pGroup = me->GetGroup();
    if (pGroup)
    {
        Group::MemberSlotList const& memList = pGroup->GetMemberSlots();
        for (Group::MemberSlot const& slot : memList)
        {
            Player* player = ObjectAccessor::FindPlayer(slot.guid);
            if (!player || !player->IsAlive() || me->GetMap() != player->GetMap() || !player->IsInWorld())
                continue;
            //if (BotGroupAI* pAI = dynamic_cast<BotGroupAI*>(me->GetAI()))
            if(PlayerBotAI* pbAI = sFakePlayerMgr->GetPlayerBotAI(player))
                if(BotBGAI* pAI = pbAI->ToBotBGAI())
                {
                    if (pAI->CastingSummonRite())
                        return 0;
                }
        }
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(WarlockIDLE_SummonRite);
    if (!spellInfo || spellInfo->IsPassive())
        return SpellCastResult::SPELL_FAILED_UNKNOWN;
    Spell* spell = new Spell(me, spellInfo, TriggerCastFlags(TriggerCastFlags::TRIGGERED_IGNORE_POWER_AND_REAGENT_COST | TriggerCastFlags::TRIGGERED_IGNORE_CAST_ITEM), ObjectGuid::Empty);
    spell->m_CastItem = NULL;
    SpellCastTargets targets;
    targets.SetUnitTarget(me);
    Dismount();
    SpellCastResult castResult = spell->prepare(targets);
    if (castResult == SPELL_CAST_OK)
        return WarlockIDLE_SummonRite;
    return 0;
}
