
#ifndef _BOT_MAGE_AI_H
#define _BOT_MAGE_AI_H

#include "ScriptSystem.h"
#include "BotAI.h"
#include "AIWaypointsMgr.h"

enum MageTalentType
{
    MageTalent_Arcane,
    MageTalent_Fire,
    MageTalent_Frost
};

class BotMageAI : public BotBGAI
{
public:
    BotMageAI(Player* player) :
        BotBGAI(player),
        m_BotTalentType(0),
        m_LastPolymorphTarget(ObjectGuid::Empty),
        m_StealthTick(0)
    {}
    ~BotMageAI() {}

    void ResetBotAI() override;

protected:
    uint32 GetManaPowerPer();
    void ClearMechanicAura() override;
    bool NeedWaitSpecialSpell(uint32 diff) override;
    void InitializeSpells();
    void UpdateTalentType();
    bool TargetCanFreeze(Unit* pTarget);
    void EachTick() override;
    bool NeedFlee() override;
    void ProcessReady() override;
    void ProcessFlee() override;
    bool ProcessNormalSpell() override;
    void ProcessMeleeSpell(Unit* pTarget) override;
    void ProcessRangeSpell(Unit* pTarget) override;

    bool ProcessArcaneFlee();
    bool ProcessFireFlee();
    bool ProcessFrostFlee();
    void ProcessArcaneRangeSpell(Unit* pTarget);
    void ProcessFireRangeSpell(Unit* pTarget);
    void ProcessFrostRangeSpell(Unit* pTarget);
    void ProcessArcaneMeleeSpell(Unit* pTarget);
    void ProcessFireMeleeSpell(Unit* pTarget);
    void ProcessFrostMeleeSpell(Unit* pTarget);
    bool ProcessPolymorph(Unit* pTarget);
    bool ProcessFreezeSpell(Unit* pTarget);

    void ProcessPetAttack(Unit* pTarget);
    bool CastingControlCommandSpell() override;
    bool TryStartControlCommand() override;
    float TryPushControlCommand(Player* pTarget) override;
    bool IsStealthing();
    void OnCastTeleport();
    void OnCastPolymorph(Unit* pTarget);
    bool TargetCanPolymorph(Unit* pTarget);
    Position GetTeleportSpellPos();
    bool UseManaJewel();
    Item* FindExistManaJewel();
    uint32 TryCastSummonRiteSpell() override;

private:
    ObjectGuid m_LastPolymorphTarget;
    uint32 m_StealthTick;
    uint32 m_BotTalentType;
    std::set<uint32> m_JewelEntrys;


    uint32 MageIDLE_ManaGem;// 42985		????????
    uint32 MageIDLE_ArcaneMagic;// 42995	????????
    uint32 MageIDLE_AOEArcaneMagic;// 43002	????????????
    uint32 MageIDLE_MgcArmor;// 43024		??????
    uint32 MageIDLE_FrostArmor;// 43008		??????
    uint32 MageIDLE_IceArmor;// 7301		??????
    uint32 MageIDLE_FireArmor;// 43046		????????
    uint32 MageIDLE_MagicAdd;// 43017		????????
    uint32 MageIDLE_MagicDec;// 43015		????????
    uint32 MageIDLE_SummonRite;// 58659		????????

    uint32 MageGuard_MagicShield;// 43020	????????
    uint32 MageGuard_FrostShield;// 43039	????????
    uint32 MageGuard_FrostScherm;// 45438	????????
    uint32 MageGuard_FrostNova;// 42917		????????
    uint32 MageGuard_FireBreath;// 42950	????????
    uint32 MageGuard_FireNova;// 42945		????????

    uint32 MageAssist_Mirror;// 55342		????????
    uint32 MageAssist_Rouse;// 12051		????
    uint32 MageAssist_Stealth;// 66			??????
    uint32 MageAssist_Teleport;// 1953		??????
    uint32 MageAssist_DecCurse;// 475		????????
    uint32 MageAssist_Grace;// 12043		????????
    uint32 MageAssist_ArcanePower;// 12042	????????
    uint32 MageAssist_CastSpeed;// 12472	????????
    uint32 MageAssist_FastColddown;// 11958	????????????CD
    uint32 MageAssist_FrostPet;// 31687		??????????
    uint32 MageAssist_FireCritAura;// 11129	????3??????????????AURA

    uint32 MageConfine_BreakCast;// 2139	????????
    uint32 MageConfine_AuraSteal;// 30449	BUFF????
    uint32 MageConfine_ArcaneSlow;// 31589	????????
    uint32 MageConfine_ToSheep;// 12826		????
    uint32 MageConfine_Freeze;// 44572		????????????????

    uint32 MageAOE_ArcaneExplode;// 42921	????????
    uint32 MageAOE_Snowstorm;// 42940		??????
    uint32 MageAOE_IcePiton;// 42931		??????
    uint32 MageAOE_FireStorm;// 42926		????????

    uint32 MageArcane_Barrage;// 44781		????????
    uint32 MageArcane_Bullet;// 42846		????????
    uint32 MageArcane_ArcaneShock;// 42897	????????

    uint32 MageFrost_IceArrow;// 42842		??????
    uint32 MageFrost_IceLance;// 42914		????
    uint32 MageFrost_FFArrow;// 47610		??????

    uint32 MageFire_FireArrow;// 42833		??????
    uint32 MageFire_FireShock;// 42873		????????
    uint32 MageFire_Firing;// 42859			????
    uint32 MageFire_BigFireBall;// 42891	??????
    uint32 MageFire_FireBomb;// 55360		????????

    uint32 MagePet_FrostNova;// 33395		??????????????

    uint32 MageFlag_FireStun;// 64343		Aura:????????????
    uint32 MageFlag_FastFStorm;// 54741		Aura:????????????
    uint32 MageFlag_FastBFBall;// 48108		Aura:??????????
    uint32 MageFlag_FastFFArrow;// 57761	Aura:??????????
    uint32 MageFlag_CanFrozen;// 74396		Aura:??????????????????
    uint32 MageFlag_Scherm;// 45438			Aura:??????????
};

#endif // !_BOT_MAGE_AI_H
