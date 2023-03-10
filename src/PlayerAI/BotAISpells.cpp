#include "BotAISpells.h"
#include "BotAITool.h"
//??
void BotWarriorSpells::InitializeSpells(Player* player)
{
    WarriorIDLE_AOEAddLife = BotUtility::FindMaxRankSpellByExist(player, 47440);
    WarriorIDLE_AOEAddPower = BotUtility::FindMaxRankSpellByExist(player, 47436);
    WarriorWeapon_Status = BotUtility::FindMaxRankSpellByExist(player, 2457);
    WarriorDefance_Status = BotUtility::FindMaxRankSpellByExist(player, 71);
    WarriorRage_Status = BotUtility::FindMaxRankSpellByExist(player, 2458);
    WarriorCommon_PowerAtt = BotUtility::FindMaxRankSpellByExist(player, 47450);
    WarriorCommon_PowerThrow = BotUtility::FindMaxRankSpellByExist(player, 57755);
    WarriorCommon_PowerRelife = BotUtility::FindMaxRankSpellByExist(player, 55694);
    WarriorCommon_ClearCtrl = BotUtility::FindMaxRankSpellByExist(player, 18499);
    WarriorCommon_AOEFear = BotUtility::FindMaxRankSpellByExist(player, 5246);
    WarriorCommon_SweepAtt = BotUtility::FindMaxRankSpellByExist(player, 47520);
    WarriorCommon_AddPower = BotUtility::FindMaxRankSpellByExist(player, 2687);
    WarriorCommon_AOEDecPower = BotUtility::FindMaxRankSpellByExist(player, 47437);
    WarriorDefance_HPojia = BotUtility::FindMaxRankSpellByExist(player, 47498);
    WarriorDefance_Fuchou = BotUtility::FindMaxRankSpellByExist(player, 57823);
    WarriorDefance_ShieldBlock = BotUtility::FindMaxRankSpellByExist(player, 2565);
    WarriorDefance_ShieldAtt = BotUtility::FindMaxRankSpellByExist(player, 47488);
    WarriorDefance_Pojia = BotUtility::FindMaxRankSpellByExist(player, 7386);
    WarriorDefance_MaxLife = BotUtility::FindMaxRankSpellByExist(player, 12975);
    WarriorDefance_ShiledWall = BotUtility::FindMaxRankSpellByExist(player, 871);
    WarriorDefance_Disarm = BotUtility::FindMaxRankSpellByExist(player, 676);
    WarriorDefance_Support = BotUtility::FindMaxRankSpellByExist(player, 3411);
    WarriorDefance_Conk = BotUtility::FindMaxRankSpellByExist(player, 12809);
    WarriorDefance_AOEConk = BotUtility::FindMaxRankSpellByExist(player, 46968);
    WarriorDefance_Pull = BotUtility::FindMaxRankSpellByExist(player, 355);
    WarriorWeapon_SwordStorm = BotUtility::FindMaxRankSpellByExist(player, 46924);
    WarriorWeaponDefance_AOEAtt = BotUtility::FindMaxRankSpellByExist(player, 47502);
    WarriorWeaponDefance_Bleed = BotUtility::FindMaxRankSpellByExist(player, 47465);
    WarriorWeaponDefance_SpellReflect = BotUtility::FindMaxRankSpellByExist(player, 23920);
    WarriorWeaponDefance_ShieldHit = BotUtility::FindMaxRankSpellByExist(player, 72);
    WarriorWeapon_HighThrow = BotUtility::FindMaxRankSpellByExist(player, 64382);
    WarriorWeapon_Charge = BotUtility::FindMaxRankSpellByExist(player, 11578);
    WarriorWeapon_Suppress = BotUtility::FindMaxRankSpellByExist(player, 7384);
    WarriorWeapon_Backstorm = BotUtility::FindMaxRankSpellByExist(player, 20230);
    WarriorWeapon_DeadAtt = BotUtility::FindMaxRankSpellByExist(player, 47486);
    WarriorWeaponRage_FullKill = BotUtility::FindMaxRankSpellByExist(player, 47471);
    WarriorWeaponRage_WinAttack = BotUtility::FindMaxRankSpellByExist(player, 34428);
    WarriorWeaponRage_Backfillet = BotUtility::FindMaxRankSpellByExist(player, 1715);
    WarriorRage_Harsh = BotUtility::FindMaxRankSpellByExist(player, 12323);
    WarriorRage_HeadAtt = BotUtility::FindMaxRankSpellByExist(player, 6552);
    WarriorRage_Intercept = BotUtility::FindMaxRankSpellByExist(player, 20252);
    WarriorRage_Whirlwind = BotUtility::FindMaxRankSpellByExist(player, 1680);
    WarriorRage_Impertinency = BotUtility::FindMaxRankSpellByExist(player, 1719);
    WarriorRage_Needdead = BotUtility::FindMaxRankSpellByExist(player, 12292);
    WarriorRage_Bloodthirsty = BotUtility::FindMaxRankSpellByExist(player, 23881);
    WarriorRage_ReIntercept = BotUtility::FindMaxRankSpellByExist(player, 60970);
}
//??
void BotPaladinSpells::InitializeSpells(Player* player)
{
    PaladinIDLE_MountAura = BotUtility::FindMaxRankSpellByExist(player, 32223);
    PaladinIDLE_CastAura = BotUtility::FindMaxRankSpellByExist(player, 19746);
    PaladinIDLE_JudgeAura = BotUtility::FindMaxRankSpellByExist(player, 54043);
    PaladinIDLE_ArmorAura = BotUtility::FindMaxRankSpellByExist(player, 48942);
    PaladinIDLE_AOEGuardWish = BotUtility::FindMaxRankSpellByExist(player, 25899);
    PaladinIDLE_GuardWish = BotUtility::FindMaxRankSpellByExist(player, 20911);
    PaladinIDLE_AOEKingWish = BotUtility::FindMaxRankSpellByExist(player, 25898);
    PaladinIDLE_KingWish = BotUtility::FindMaxRankSpellByExist(player, 20217);
    PaladinIDLE_AOEWitWish = BotUtility::FindMaxRankSpellByExist(player, 48938);
    PaladinIDLE_WitWish = BotUtility::FindMaxRankSpellByExist(player, 48936);
    PaladinIDLE_AOEStrWish = BotUtility::FindMaxRankSpellByExist(player, 48934);
    PaladinIDLE_StrWish = BotUtility::FindMaxRankSpellByExist(player, 48932);
    PaladinIDLE_JusticeRage = BotUtility::FindMaxRankSpellByExist(player, 25780);
    PaladinIDLE_Revive = BotUtility::FindMaxRankSpellByExist(player, 48950);

    PaladinGuard_UnShield = BotUtility::FindMaxRankSpellByExist(player, 53601);
    PaladinGuard_FreeAura = BotUtility::FindMaxRankSpellByExist(player, 1044);
    PaladinGuard_Invincible = BotUtility::FindMaxRankSpellByExist(player, 642);
    PaladinGuard_Sacrifice = BotUtility::FindMaxRankSpellByExist(player, 6940);
    PaladinGuard_AOESacrifice = BotUtility::FindMaxRankSpellByExist(player, 64205);
    PaladinGuard_BlockShield = BotUtility::FindMaxRankSpellByExist(player, 48952);
    PaladinGuard_PhyImmune = BotUtility::FindMaxRankSpellByExist(player, 10278);
    PaladinGuard_Pull = BotUtility::FindMaxRankSpellByExist(player, 62124);

    PaladinAssist_UpPower = BotUtility::FindMaxRankSpellByExist(player, 31884);
    PaladinAssist_RevengeStamp = BotUtility::FindMaxRankSpellByExist(player, 31801);
    PaladinAssist_LifeStamp = BotUtility::FindMaxRankSpellByExist(player, 20165);
    PaladinAssist_ManaStamp = BotUtility::FindMaxRankSpellByExist(player, 20166);
    PaladinAssist_JusticeStamp = BotUtility::FindMaxRankSpellByExist(player, 21084);
    PaladinAssist_StunStamp = BotUtility::FindMaxRankSpellByExist(player, 20164);
    PaladinAssist_ComStamp = BotUtility::FindMaxRankSpellByExist(player, 20375);
    PaladinAssist_Confession = BotUtility::FindMaxRankSpellByExist(player, 20066);
    PaladinAssist_StunMace = BotUtility::FindMaxRankSpellByExist(player, 10308);
    PaladinAssist_ReviveMana = BotUtility::FindMaxRankSpellByExist(player, 54428);
    PaladinAssist_HealCrit = BotUtility::FindMaxRankSpellByExist(player, 20216);
    PaladinAssist_LowMana = BotUtility::FindMaxRankSpellByExist(player, 31842);
    PaladinAssist_AuraUP = BotUtility::FindMaxRankSpellByExist(player, 31821);
    PaladinAssist_Dispel = BotUtility::FindMaxRankSpellByExist(player, 4987);

    PaladinHeal_FastHoly = BotUtility::FindMaxRankSpellByExist(player, 48785);
    PaladinHeal_BigHoly = BotUtility::FindMaxRankSpellByExist(player, 48782);
    PaladinHeal_FullHoly = BotUtility::FindMaxRankSpellByExist(player, 48788);

    PaladinMelee_AOEOffertory = BotUtility::FindMaxRankSpellByExist(player, 48819);
    PaladinMelee_KillMace = BotUtility::FindMaxRankSpellByExist(player, 48806);
    PaladinMelee_FlyShield = BotUtility::FindMaxRankSpellByExist(player, 48827);
    PaladinMelee_ShieldAtt = BotUtility::FindMaxRankSpellByExist(player, 61411);
    PaladinMelee_MaceAtt = BotUtility::FindMaxRankSpellByExist(player, 53595);
    PaladinMelee_HolyAtt = BotUtility::FindMaxRankSpellByExist(player, 48825);
    PaladinMelee_LifeJudge = BotUtility::FindMaxRankSpellByExist(player, 20271);
    PaladinMelee_ManaJudge = BotUtility::FindMaxRankSpellByExist(player, 53408);
    PaladinMelee_FleeJudge = BotUtility::FindMaxRankSpellByExist(player, 53407);
    PaladinMelee_WeaponAtt = BotUtility::FindMaxRankSpellByExist(player, 35395);
    PaladinMelee_HolyStrom = BotUtility::FindMaxRankSpellByExist(player, 53385);

    PaladinFlag_MomentHoly = 59578;
    PaladinFlag_Discipline = 25771;
}

void BotDeathknightSpells::InitializeSpells(Player* player)
{
    DKStatus_Frost = BotUtility::FindMaxRankSpellByExist(player, 48263);
    DKStatus_Evil = BotUtility::FindMaxRankSpellByExist(player, 48265);
    DKStatus_Blood = BotUtility::FindMaxRankSpellByExist(player, 48266);

    DKIDLE_Buffer = BotUtility::FindMaxRankSpellByExist(player, 57623);
    DKIDLE_SummonPet = BotUtility::FindMaxRankSpellByExist(player, 46584);
    DKIDLE_SummonAllPets = BotUtility::FindMaxRankSpellByExist(player, 42650);

    DKBlock_Silence = BotUtility::FindMaxRankSpellByExist(player, 47476);
    DKBlock_Cast = BotUtility::FindMaxRankSpellByExist(player, 47528);

    DKPulls_Pull = BotUtility::FindMaxRankSpellByExist(player, 56222);
    DKPulls_DKPull = BotUtility::FindMaxRankSpellByExist(player, 49576);

    DKDefense_MgcShield = BotUtility::FindMaxRankSpellByExist(player, 48707);
    DKDefense_NoMgcArea = BotUtility::FindMaxRankSpellByExist(player, 51052);
    DKDefense_Contract = BotUtility::FindMaxRankSpellByExist(player, 48743);
    DKDefense_IceBody = BotUtility::FindMaxRankSpellByExist(player, 48792);
    DKDefense_IceArmor = BotUtility::FindMaxRankSpellByExist(player, 51271);
    DKDefense_BoneShield = BotUtility::FindMaxRankSpellByExist(player, 49222);

    DKAssist_RuneLife = BotUtility::FindMaxRankSpellByExist(player, 48982);
    DKAssist_BloodBrand = BotUtility::FindMaxRankSpellByExist(player, 49005);
    DKAssist_Frenzied = BotUtility::FindMaxRankSpellByExist(player, 49016);
    DKAssist_BloodBuf = BotUtility::FindMaxRankSpellByExist(player, 55233);
    DKAssist_RuneWeapon = BotUtility::FindMaxRankSpellByExist(player, 47568);
    DKAssist_Infect = BotUtility::FindMaxRankSpellByExist(player, 50842);
    DKAssist_RuneShunt = BotUtility::FindMaxRankSpellByExist(player, 45529);
    DKAssist_IceLock = BotUtility::FindMaxRankSpellByExist(player, 45524);
    DKAssist_DeadRevive = BotUtility::FindMaxRankSpellByExist(player, 49895);
    DKAssist_NonFear = BotUtility::FindMaxRankSpellByExist(player, 49039);
    DKAssist_NextCrit = BotUtility::FindMaxRankSpellByExist(player, 49796);
    DKAssist_EatIce = BotUtility::FindMaxRankSpellByExist(player, 49203);
    DKAssist_PetPower = BotUtility::FindMaxRankSpellByExist(player, 63560);
    DKAssist_SummonFlyAtt = BotUtility::FindMaxRankSpellByExist(player, 49206);
    DKAssist_SummonRuneWeapon = BotUtility::FindMaxRankSpellByExist(player, 49028);

    DKAttack_IceSickness = BotUtility::FindMaxRankSpellByExist(player, 49909);
    DKAttack_NearAOE = BotUtility::FindMaxRankSpellByExist(player, 49941);
    DKAttack_AreaAOE = BotUtility::FindMaxRankSpellByExist(player, 49938);
    DKAttack_BloodAtt = BotUtility::FindMaxRankSpellByExist(player, 49930);
    DKAttack_ShadowAtt = BotUtility::FindMaxRankSpellByExist(player, 49921);
    DKAttack_FrostAtt = BotUtility::FindMaxRankSpellByExist(player, 55268);
    DKAttack_DoDestroy = BotUtility::FindMaxRankSpellByExist(player, 51425);
    DKAttack_RuneAttack = BotUtility::FindMaxRankSpellByExist(player, 56815);
    DKAttack_LifeAttack = BotUtility::FindMaxRankSpellByExist(player, 49924);
    DKAttack_IceWindAtt = BotUtility::FindMaxRankSpellByExist(player, 51411);
    DKAttack_CorpseExplosion = BotUtility::FindMaxRankSpellByExist(player, 51328);
    DKAttack_NaturalAtt = BotUtility::FindMaxRankSpellByExist(player, 55271);
    DKAttack_CoreAtt = BotUtility::FindMaxRankSpellByExist(player, 55262);
}
//??
void BotRogueSpells::InitializeSpells(Player* player)
{
    RogueGuard_Sneak = BotUtility::FindMaxRankSpellByExist(player, 1784);
    RogueGuard_ShadowCloak = BotUtility::FindMaxRankSpellByExist(player, 31224);
    RogueGuard_Disappear = BotUtility::FindMaxRankSpellByExist(player, 26889);
    RogueGuard_Dodge = BotUtility::FindMaxRankSpellByExist(player, 26669);
    RogueGuard_Sprint = BotUtility::FindMaxRankSpellByExist(player, 11305);

    RogueSneak_Stick = BotUtility::FindMaxRankSpellByExist(player, 51724);
    RogueSneak_Premeditate = BotUtility::FindMaxRankSpellByExist(player, 14183);
    RogueSneak_Ambush = BotUtility::FindMaxRankSpellByExist(player, 48691);
    RogueSneak_Surprise = BotUtility::FindMaxRankSpellByExist(player, 1833);

    RogueAssist_ShadowDance = BotUtility::FindMaxRankSpellByExist(player, 51713);
    RogueAssist_ShadowFlash = BotUtility::FindMaxRankSpellByExist(player, 36554);
    RogueAssist_ReadyCD = BotUtility::FindMaxRankSpellByExist(player, 14185);
    RogueAssist_Blind = BotUtility::FindMaxRankSpellByExist(player, 2094);
    RogueAssist_Disarm = BotUtility::FindMaxRankSpellByExist(player, 51722);
    RogueAssist_NextCrit = BotUtility::FindMaxRankSpellByExist(player, 14177);
    RogueAssist_blood = BotUtility::FindMaxRankSpellByExist(player, 51662);
    RogueAssist_FastEnergy = BotUtility::FindMaxRankSpellByExist(player, 13750);
    RogueAssist_BlockCast = BotUtility::FindMaxRankSpellByExist(player, 1766);
    RogueAssist_Paralyze = BotUtility::FindMaxRankSpellByExist(player, 1776);
    RogueAssist_FastSpeed = BotUtility::FindMaxRankSpellByExist(player, 13877);

    RogueAOE_Knife = BotUtility::FindMaxRankSpellByExist(player, 51723);
    RogueAOE_AllDance = BotUtility::FindMaxRankSpellByExist(player, 51690);

    RogueAttack_Blood = BotUtility::FindMaxRankSpellByExist(player, 48660);
    RogueAttack_Ghost = BotUtility::FindMaxRankSpellByExist(player, 14278);
    RogueAttack_Injure = BotUtility::FindMaxRankSpellByExist(player, 48666);
    RogueAttack_PoisonAtt = BotUtility::FindMaxRankSpellByExist(player, 5938);
    RogueAttack_BackAtt = BotUtility::FindMaxRankSpellByExist(player, 48657);
    RogueAttack_EvilAtt = BotUtility::FindMaxRankSpellByExist(player, 48638);

    RogueAttack_Damage = BotUtility::FindMaxRankSpellByExist(player, 48668);
    RogueAttack_Separate = BotUtility::FindMaxRankSpellByExist(player, 48672);
    RogueAttack_Stun = BotUtility::FindMaxRankSpellByExist(player, 8643);
    RogueAttack_PoisonDmg = BotUtility::FindMaxRankSpellByExist(player, 57993);
    RogueAttack_Incision = BotUtility::FindMaxRankSpellByExist(player, 6774);
    RogueRange_Throw = BotUtility::FindMaxRankSpellByExist(player, 48674);

    RogueFlag_Dance = 51713;
}

void BotDruidSpells::InitializeSpells(Player* player)
{
    DruidIDLE_FerityWish = BotUtility::FindMaxRankSpellByExist(player, 48469);
    DruidIDLE_AOEFerityWish = BotUtility::FindMaxRankSpellByExist(player, 48470);
    DruidIDLE_Revive = BotUtility::FindMaxRankSpellByExist(player, 50769);
    DruidIDLE_CombatReive = BotUtility::FindMaxRankSpellByExist(player, 20484);

    DruidStatus_Travel = BotUtility::FindMaxRankSpellByExist(player, 783);
    DruidStatus_Bear = BotUtility::FindMaxRankSpellByExist(player, 9634);
    DruidStatus_Cat = BotUtility::FindMaxRankSpellByExist(player, 768);
    DruidStatus_Bird = BotUtility::FindMaxRankSpellByExist(player, 24858);
    DruidStatus_Tree = BotUtility::FindMaxRankSpellByExist(player, 33891);

    DruidGuard_Sneak = BotUtility::FindMaxRankSpellByExist(player, 5215);
    DruidGuard_Harden = BotUtility::FindMaxRankSpellByExist(player, 22812);
    DruidGuard_Thorns = BotUtility::FindMaxRankSpellByExist(player, 53307);
    DruidGuard_AutoTwine = BotUtility::FindMaxRankSpellByExist(player, 53312);
    DruidGuard_Twine = BotUtility::FindMaxRankSpellByExist(player, 53308);
    DruidGuard_Control = BotUtility::FindMaxRankSpellByExist(player, 33786);
    DruidGuard_Pofu = BotUtility::FindMaxRankSpellByExist(player, 61336);
    DruidGuard_TreeMan = BotUtility::FindMaxRankSpellByExist(player, 33831);

    DruidAssist_PersonSpirit = BotUtility::FindMaxRankSpellByExist(player, 770);
    DruidAssist_BeastSpirit = BotUtility::FindMaxRankSpellByExist(player, 16857);
    DruidAssist_Active = BotUtility::FindMaxRankSpellByExist(player, 29166);
    DruidAssist_DecCruse = BotUtility::FindMaxRankSpellByExist(player, 2782);
    DruidAssist_DecCruel = BotUtility::FindMaxRankSpellByExist(player, 2893);

    DruidCast_Moonfire = BotUtility::FindMaxRankSpellByExist(player, 48463);
    DruidCast_Insect = BotUtility::FindMaxRankSpellByExist(player, 48468);
    DruidCast_Anger = BotUtility::FindMaxRankSpellByExist(player, 48461);
    DruidCast_Spark = BotUtility::FindMaxRankSpellByExist(player, 48465);

    DruidAOE_Hurricane = BotUtility::FindMaxRankSpellByExist(player, 48467);
    DruidAOE_Typhoon = BotUtility::FindMaxRankSpellByExist(player, 61384);
    DruidAOE_FallStar = BotUtility::FindMaxRankSpellByExist(player, 48505);

    DruidHeal_Nourishing = BotUtility::FindMaxRankSpellByExist(player, 50464);
    DruidHeal_Relife = BotUtility::FindMaxRankSpellByExist(player, 48441);
    DruidHeal_Coalescence = BotUtility::FindMaxRankSpellByExist(player, 48443);
    DruidHeal_Touch = BotUtility::FindMaxRankSpellByExist(player, 48378);
    DruidHeal_LifeBurst = BotUtility::FindMaxRankSpellByExist(player, 48451);
    DruidHeal_MergerLife = BotUtility::FindMaxRankSpellByExist(player, 18562);
    DruidHeal_MomentHeal = BotUtility::FindMaxRankSpellByExist(player, 17116);

    DruidHeal_AOETranquility = BotUtility::FindMaxRankSpellByExist(player, 48447);
    DruidHeal_AOEFerity = BotUtility::FindMaxRankSpellByExist(player, 53251);

    DruidCat_Stun = BotUtility::FindMaxRankSpellByExist(player, 49802);
    DruidCat_Bite = BotUtility::FindMaxRankSpellByExist(player, 48577);
    DruidCat_Roar = BotUtility::FindMaxRankSpellByExist(player, 52610);
    DruidCat_Separate = BotUtility::FindMaxRankSpellByExist(player, 49800);

    DruidCat_Tiger = BotUtility::FindMaxRankSpellByExist(player, 50213);
    DruidCat_FastMove = BotUtility::FindMaxRankSpellByExist(player, 33357);
    DruidCat_Charge = BotUtility::FindMaxRankSpellByExist(player, 49376);
    DruidCat_Surprise = BotUtility::FindMaxRankSpellByExist(player, 49803);
    DruidCat_Sack = BotUtility::FindMaxRankSpellByExist(player, 48579);
    DruidCat_Claw = BotUtility::FindMaxRankSpellByExist(player, 48570);
    DruidCat_BackStab = BotUtility::FindMaxRankSpellByExist(player, 48572);
    DruidCat_Attack = BotUtility::FindMaxRankSpellByExist(player, 62078);
    DruidCat_Sweep = BotUtility::FindMaxRankSpellByExist(player, 48574);
    DruidCat_Laceration = BotUtility::FindMaxRankSpellByExist(player, 48566);

    DruidBear_DecAtt = BotUtility::FindMaxRankSpellByExist(player, 48560);
    DruidBear_AddPower = BotUtility::FindMaxRankSpellByExist(player, 5229);
    DruidBear_PowerLife = BotUtility::FindMaxRankSpellByExist(player, 22842);
    DruidBear_Laceration = BotUtility::FindMaxRankSpellByExist(player, 48564);
    DruidBear_Sweep = BotUtility::FindMaxRankSpellByExist(player, 48568);
    DruidBear_Attack = BotUtility::FindMaxRankSpellByExist(player, 48562);
    DruidBear_NextAtt = BotUtility::FindMaxRankSpellByExist(player, 48480);
    DruidBear_Stun = BotUtility::FindMaxRankSpellByExist(player, 8983);
    DruidBear_Charge = BotUtility::FindMaxRankSpellByExist(player, 16979);
}

void BotHunterSpells::InitializeSpells(Player* player)
{
    HunterIDLE_SummonPet = BotUtility::FindMaxRankSpellByExist(player, 883);
    HunterIDLE_RevivePet = BotUtility::FindMaxRankSpellByExist(player, 982);
    HunterIDLE_ManaAura = BotUtility::FindMaxRankSpellByExist(player, 34074);
    HunterIDLE_DodgeAura = BotUtility::FindMaxRankSpellByExist(player, 13163);
    HunterIDLE_EagleAura = BotUtility::FindMaxRankSpellByExist(player, 27044);
    HunterIDLE_DragonAura = BotUtility::FindMaxRankSpellByExist(player, 61847);
    HunterIDLE_ShotAura = BotUtility::FindMaxRankSpellByExist(player, 19506);

    HunterTrap_FarFrozen = BotUtility::FindMaxRankSpellByExist(player, 60192);
    HunterTrap_Frozen = BotUtility::FindMaxRankSpellByExist(player, 14311);
    HunterTrap_Ice = BotUtility::FindMaxRankSpellByExist(player, 13809);
    HunterTrap_Viper = BotUtility::FindMaxRankSpellByExist(player, 34600);
    HunterTrap_Explode = BotUtility::FindMaxRankSpellByExist(player, 49067);
    HunterTrap_Fire = BotUtility::FindMaxRankSpellByExist(player, 49056);
    HunterTrap_Shot = BotUtility::FindMaxRankSpellByExist(player, 63672);

    HunterAssist_ClearRoot = BotUtility::FindMaxRankSpellByExist(player, 53271);
    HunterAssist_PetCommand = BotUtility::FindMaxRankSpellByExist(player, 34026);
    HunterAssist_HealPet = BotUtility::FindMaxRankSpellByExist(player, 48990);
    HunterAssist_PetStun = BotUtility::FindMaxRankSpellByExist(player, 19577);
    HunterAssist_PetRage = BotUtility::FindMaxRankSpellByExist(player, 19574);
    HunterAssist_Stamp = BotUtility::FindMaxRankSpellByExist(player, 53338);
    HunterAssist_FalseDead = BotUtility::FindMaxRankSpellByExist(player, 5384);
    HunterAssist_BackJump = BotUtility::FindMaxRankSpellByExist(player, 781);
    HunterAssist_FastSpeed = BotUtility::FindMaxRankSpellByExist(player, 3045);
    HunterAssist_ReadyCD = BotUtility::FindMaxRankSpellByExist(player, 23989);
    HunterAssist_Mislead = BotUtility::FindMaxRankSpellByExist(player, 34477);

    HunterMelee_BackRoot = BotUtility::FindMaxRankSpellByExist(player, 48999);
    HunterMelee_NoDamage = BotUtility::FindMaxRankSpellByExist(player, 19263);
    HunterMelee_DecSpeed = BotUtility::FindMaxRankSpellByExist(player, 2974);
    HunterMelee_NextAtt = BotUtility::FindMaxRankSpellByExist(player, 48996);
    HunterMelee_MeleeAtt = BotUtility::FindMaxRankSpellByExist(player, 53339);

    HunterDebug_Damage = BotUtility::FindMaxRankSpellByExist(player, 49001);
    HunterDebug_Mana = BotUtility::FindMaxRankSpellByExist(player, 3034);
    HunterDebug_Sleep = BotUtility::FindMaxRankSpellByExist(player, 49012);

    HunterShot_AOEShot = BotUtility::FindMaxRankSpellByExist(player, 58434);
    HunterShot_CharmShot = BotUtility::FindMaxRankSpellByExist(player, 19503);
    HunterShot_Explode = BotUtility::FindMaxRankSpellByExist(player, 60053);
    HunterShot_Aim = BotUtility::FindMaxRankSpellByExist(player, 49050);
    HunterShot_Silence = BotUtility::FindMaxRankSpellByExist(player, 34490);
    HunterShot_Shock = BotUtility::FindMaxRankSpellByExist(player, 5116);
    HunterShot_Cast = BotUtility::FindMaxRankSpellByExist(player, 49052);
    HunterShot_MgcShot = BotUtility::FindMaxRankSpellByExist(player, 49045);
    HunterShot_KillShot = BotUtility::FindMaxRankSpellByExist(player, 61006);
    HunterShot_MulShot = BotUtility::FindMaxRankSpellByExist(player, 49048);
    HunterShot_QMLShot = BotUtility::FindMaxRankSpellByExist(player, 53209);
}

void BotShamanSpells::InitializeSpells(Player* player)
{
    ShamanIDLE_LifeWeapon = BotUtility::FindMaxRankSpellByExist(player, 51994);
    ShamanIDLE_IceWeapon = BotUtility::FindMaxRankSpellByExist(player, 58796);
    ShamanIDLE_FireWeapon = BotUtility::FindMaxRankSpellByExist(player, 58790);
    ShamanIDLE_PhyWeapon = BotUtility::FindMaxRankSpellByExist(player, 10399);
    ShamanIDLE_FastWeapon = BotUtility::FindMaxRankSpellByExist(player, 58804);
    ShamanIDLE_Revive = BotUtility::FindMaxRankSpellByExist(player, 49277);

    ShamanShield_Earth = BotUtility::FindMaxRankSpellByExist(player, 49284);
    ShamanShield_Water = BotUtility::FindMaxRankSpellByExist(player, 57960);
    ShamanShield_Lightning = BotUtility::FindMaxRankSpellByExist(player, 49281);

    ShamanAssist_Frog = BotUtility::FindMaxRankSpellByExist(player, 51514);
    ShamanAssist_HealCrit = BotUtility::FindMaxRankSpellByExist(player, 55198);
    ShamanAssist_MomentHeal = BotUtility::FindMaxRankSpellByExist(player, 16188);
    ShamanAssist_MomentCast = BotUtility::FindMaxRankSpellByExist(player, 16166);
    ShamanAssist_BlockCast = BotUtility::FindMaxRankSpellByExist(player, 57994);
    ShamanAssist_Cleansing = BotUtility::FindMaxRankSpellByExist(player, 8012);
    ShamanAssist_FireNova = BotUtility::FindMaxRankSpellByExist(player, 61657);
    ShamanAssist_Heroic = BotUtility::FindMaxRankSpellByExist(player, 32182);
    ShamanAssist_DecCruel = BotUtility::FindMaxRankSpellByExist(player, 526);

    ShamanAtt_StormStrike = BotUtility::FindMaxRankSpellByExist(player, 17364);
    ShamanAtt_FireStrike = BotUtility::FindMaxRankSpellByExist(player, 60103);

    ShamanCast_LightningArrow = BotUtility::FindMaxRankSpellByExist(player, 49238);
    ShamanCast_LightningChain = BotUtility::FindMaxRankSpellByExist(player, 49271);
    ShamanCast_LightningStorm = BotUtility::FindMaxRankSpellByExist(player, 59159);
    ShamanCast_FireThud = BotUtility::FindMaxRankSpellByExist(player, 49233);
    ShamanCast_IceThud = BotUtility::FindMaxRankSpellByExist(player, 49236);
    ShamanCast_EarthThud = BotUtility::FindMaxRankSpellByExist(player, 49231);
    ShamanCast_FireStrike = BotUtility::FindMaxRankSpellByExist(player, 60043);

    ShamanHealth_Fast = BotUtility::FindMaxRankSpellByExist(player, 49276);
    ShamanHealth_Bast = BotUtility::FindMaxRankSpellByExist(player, 49273);
    ShamanHealth_Chain = BotUtility::FindMaxRankSpellByExist(player, 55459);
    ShamanHealth_Torrent = BotUtility::FindMaxRankSpellByExist(player, 61301);
    ShamanDispel_Refine = BotUtility::FindMaxRankSpellByExist(player, 51886);

    ShamanTotem_Recycle = BotUtility::FindMaxRankSpellByExist(player, 36936);

    ShamanTotem_Life = BotUtility::FindMaxRankSpellByExist(player, 58757);
    ShamanTotem_Mana = BotUtility::FindMaxRankSpellByExist(player, 58774);
    ShamanTotem_BMana = BotUtility::FindMaxRankSpellByExist(player, 16190);

    ShamanTotem_SummonFire = BotUtility::FindMaxRankSpellByExist(player, 2894);
    ShamanTotem_MgcPower = BotUtility::FindMaxRankSpellByExist(player, 57722);
    ShamanTotem_Attack = BotUtility::FindMaxRankSpellByExist(player, 58704);
    ShamanTotem_AOEAttack = BotUtility::FindMaxRankSpellByExist(player, 58734);
    ShamanTotem_MgcHeal = BotUtility::FindMaxRankSpellByExist(player, 58656);

    ShamanTotem_DecMove = BotUtility::FindMaxRankSpellByExist(player, 2484);
    ShamanTotem_SummonSoil = BotUtility::FindMaxRankSpellByExist(player, 2062);
    ShamanTotem_PhyPower = BotUtility::FindMaxRankSpellByExist(player, 58643);
    ShamanTotem_Armor = BotUtility::FindMaxRankSpellByExist(player, 58753);

    ShamanTotem_AbsorbBuff = BotUtility::FindMaxRankSpellByExist(player, 8177);
    ShamanTotem_AttSpeed = BotUtility::FindMaxRankSpellByExist(player, 8512);
    ShamanTotem_MgcSpeed = BotUtility::FindMaxRankSpellByExist(player, 3738);

    ShamanFlag_NoHeroic = 57723;
}

void BotMageSpells::InitializeSpells(Player* player)
{
    MageIDLE_ManaGem = BotUtility::FindMaxRankSpellByExist(player, 42985);
    MageIDLE_ArcaneMagic = BotUtility::FindMaxRankSpellByExist(player, 42995);
    MageIDLE_AOEArcaneMagic = BotUtility::FindMaxRankSpellByExist(player, 43002);
    MageIDLE_MgcArmor = BotUtility::FindMaxRankSpellByExist(player, 43024);
    MageIDLE_FrostArmor = BotUtility::FindMaxRankSpellByExist(player, 43008);
    MageIDLE_IceArmor = BotUtility::FindMaxRankSpellByExist(player, 7301);
    MageIDLE_FireArmor = BotUtility::FindMaxRankSpellByExist(player, 43046);
    MageIDLE_MagicAdd = BotUtility::FindMaxRankSpellByExist(player, 43017);
    MageIDLE_MagicDec = BotUtility::FindMaxRankSpellByExist(player, 43015);
    MageIDLE_SummonRite = BotUtility::FindMaxRankSpellByExist(player, 58659);

    MageGuard_MagicShield = BotUtility::FindMaxRankSpellByExist(player, 43020);
    MageGuard_FrostShield = BotUtility::FindMaxRankSpellByExist(player, 43039);
    MageGuard_FrostScherm = BotUtility::FindMaxRankSpellByExist(player, 45438);
    MageGuard_FrostNova = BotUtility::FindMaxRankSpellByExist(player, 42917);
    MageGuard_FireBreath = BotUtility::FindMaxRankSpellByExist(player, 42950);
    MageGuard_FireNova = BotUtility::FindMaxRankSpellByExist(player, 42945);

    MageAssist_Mirror = BotUtility::FindMaxRankSpellByExist(player, 55342);
    MageAssist_Rouse = BotUtility::FindMaxRankSpellByExist(player, 12051);
    MageAssist_Stealth = BotUtility::FindMaxRankSpellByExist(player, 66);
    MageAssist_Teleport = BotUtility::FindMaxRankSpellByExist(player, 1953);
    MageAssist_DecCurse = BotUtility::FindMaxRankSpellByExist(player, 475);
    MageAssist_Grace = BotUtility::FindMaxRankSpellByExist(player, 12043);
    MageAssist_ArcanePower = BotUtility::FindMaxRankSpellByExist(player, 12042);
    MageAssist_CastSpeed = BotUtility::FindMaxRankSpellByExist(player, 12472);
    MageAssist_FastColddown = BotUtility::FindMaxRankSpellByExist(player, 11958);
    MageAssist_FrostPet = BotUtility::FindMaxRankSpellByExist(player, 31687);
    MageAssist_FireCritAura = BotUtility::FindMaxRankSpellByExist(player, 11129);

    MageConfine_BreakCast = BotUtility::FindMaxRankSpellByExist(player, 2139);
    MageConfine_AuraSteal = BotUtility::FindMaxRankSpellByExist(player, 30449);
    MageConfine_ArcaneSlow = BotUtility::FindMaxRankSpellByExist(player, 31589);
    MageConfine_ToSheep = BotUtility::FindMaxRankSpellByExist(player, 12826);
    MageConfine_Freeze = BotUtility::FindMaxRankSpellByExist(player, 44572);

    MageAOE_ArcaneExplode = BotUtility::FindMaxRankSpellByExist(player, 42921);
    MageAOE_Snowstorm = BotUtility::FindMaxRankSpellByExist(player, 42940);
    MageAOE_IcePiton = BotUtility::FindMaxRankSpellByExist(player, 42931);
    MageAOE_FireStorm = BotUtility::FindMaxRankSpellByExist(player, 42926);

    MageArcane_Barrage = BotUtility::FindMaxRankSpellByExist(player, 44781);
    MageArcane_Bullet = BotUtility::FindMaxRankSpellByExist(player, 42846);
    MageArcane_ArcaneShock = BotUtility::FindMaxRankSpellByExist(player, 42897);
    MageFrost_IceArrow = BotUtility::FindMaxRankSpellByExist(player, 42842);
    MageFrost_IceLance = BotUtility::FindMaxRankSpellByExist(player, 42914);
    MageFrost_FFArrow = BotUtility::FindMaxRankSpellByExist(player, 47610);
    MageFire_FireArrow = BotUtility::FindMaxRankSpellByExist(player, 42833);
    MageFire_FireShock = BotUtility::FindMaxRankSpellByExist(player, 42873);
    MageFire_Firing = BotUtility::FindMaxRankSpellByExist(player, 42859);
    MageFire_BigFireBall = BotUtility::FindMaxRankSpellByExist(player, 42891);
    MageFire_FireBomb = BotUtility::FindMaxRankSpellByExist(player, 55360);

    MagePet_FrostNova = 33395;
    MageFlag_FireStun = 64343;
    MageFlag_FastFStorm = 54741;
    MageFlag_FastBFBall = 48108;
    MageFlag_FastFFArrow = 57761;
    MageFlag_CanFrozen = 74396;
    MageFlag_Scherm = 45438;
}

void BotWarlockSpells::InitializeSpells(Player* player)
{
    WarlockIDLE_LowArmor = BotUtility::FindMaxRankSpellByExist(player, 696);
    WarlockIDLE_Armor = BotUtility::FindMaxRankSpellByExist(player, 47889);
    WarlockIDLE_HighArmor = BotUtility::FindMaxRankSpellByExist(player, 47893);
    WarlockIDLE_SoulLink = BotUtility::FindMaxRankSpellByExist(player, 19028);
    WarlockIDLE_ShadowShield = BotUtility::FindMaxRankSpellByExist(player, 47891);
    WarlockIDLE_SummonFireDemon = BotUtility::FindMaxRankSpellByExist(player, 688);
    WarlockIDLE_SummonHollowDemon = BotUtility::FindMaxRankSpellByExist(player, 697);
    WarlockIDLE_SummonSuccubus = BotUtility::FindMaxRankSpellByExist(player, 712);
    WarlockIDLE_SummonDogDemon = BotUtility::FindMaxRankSpellByExist(player, 691);
    WarlockIDLE_SummonGuardDemon = BotUtility::FindMaxRankSpellByExist(player, 30146);
    WarlockIDLE_FastSummon = BotUtility::FindMaxRankSpellByExist(player, 18708);
    WarlockIDLE_OpenGate = BotUtility::FindMaxRankSpellByExist(player, 48018);
    WarlockIDLE_TeleGate = BotUtility::FindMaxRankSpellByExist(player, 48020);
    WarlockIDLE_SummonRite = BotUtility::FindMaxRankSpellByExist(player, 29893);

    WarlockDemon_ToDemon = BotUtility::FindMaxRankSpellByExist(player, 47241);
    WarlockDemon_Charge = BotUtility::FindMaxRankSpellByExist(player, 54785);
    WarlockDemon_MeleeAOE = BotUtility::FindMaxRankSpellByExist(player, 50581);
    WarlockDemon_Sacrifice = BotUtility::FindMaxRankSpellByExist(player, 50589);

    WarlockAssist_DemonPower = BotUtility::FindMaxRankSpellByExist(player, 47193);
    WarlockAssist_ExtractMana = BotUtility::FindMaxRankSpellByExist(player, 59092);
    WarlockAssist_ConvertMana = BotUtility::FindMaxRankSpellByExist(player, 57946);
    WarlockAssist_StealLife = BotUtility::FindMaxRankSpellByExist(player, 47857);
    WarlockAssist_StealMana = BotUtility::FindMaxRankSpellByExist(player, 5138);
    WarlockAssist_BaseFear = BotUtility::FindMaxRankSpellByExist(player, 6215);
    WarlockAssist_FastFear = BotUtility::FindMaxRankSpellByExist(player, 47860);
    WarlockAssist_AOEFear = BotUtility::FindMaxRankSpellByExist(player, 17928);

    WarlockAOE_MeleeFire = BotUtility::FindMaxRankSpellByExist(player, 61290);
    WarlockAOE_RainFire = BotUtility::FindMaxRankSpellByExist(player, 47820);
    WarlockAOE_ShadowRage = BotUtility::FindMaxRankSpellByExist(player, 47847);

    WarlockCurse_UpDmg = BotUtility::FindMaxRankSpellByExist(player, 47865);
    WarlockCurse_MoveLow = BotUtility::FindMaxRankSpellByExist(player, 18223);
    WarlockCurse_MgcDmg = BotUtility::FindMaxRankSpellByExist(player, 47864);
    WarlockCurse_MeleeLow = BotUtility::FindMaxRankSpellByExist(player, 50511);
    WarlockCurse_CastLow = BotUtility::FindMaxRankSpellByExist(player, 11719);

    WarlockDot_LeechSoul = BotUtility::FindMaxRankSpellByExist(player, 59164);
    WarlockDot_HighDmg = BotUtility::FindMaxRankSpellByExist(player, 47843);
    WarlockDot_LowDmg = BotUtility::FindMaxRankSpellByExist(player, 47813);
    WarlockDot_AOEDmg = BotUtility::FindMaxRankSpellByExist(player, 47836);
    WarlockDot_Sacrifice = BotUtility::FindMaxRankSpellByExist(player, 47811);

    WarlockCast_ShadowArrow = BotUtility::FindMaxRankSpellByExist(player, 47809);
    WarlockCast_ShadowShock = BotUtility::FindMaxRankSpellByExist(player, 47827);
    WarlockCast_ChaosArrow = BotUtility::FindMaxRankSpellByExist(player, 59172);
    WarlockCast_FullBurn = BotUtility::FindMaxRankSpellByExist(player, 47838);
    WarlockCast_FireBurn = BotUtility::FindMaxRankSpellByExist(player, 17962);
    WarlockCast_BigFireBall = BotUtility::FindMaxRankSpellByExist(player, 47825);

    WarlockFlag_SoulItem = 6265;
    WarlockFlag_SoulLink = 25228;
    WarlockFlag_OpenGate = 48018;
    WarlockFlag_Sacrifice = 50589;
}

void BotPriestSpells::InitializeSpells(Player* player)
{
    PriestIDLE_AllHardRes = BotUtility::FindMaxRankSpellByExist(player, 48162);
    PriestIDLE_HardRes = BotUtility::FindMaxRankSpellByExist(player, 48161);
    PriestIDLE_SoulFire = BotUtility::FindMaxRankSpellByExist(player, 48168);
    PriestIDLE_AllSpiritRes = BotUtility::FindMaxRankSpellByExist(player, 48074);
    PriestIDLE_SpiritRes = BotUtility::FindMaxRankSpellByExist(player, 48073);
    PriestIDLE_Bloodsucker = BotUtility::FindMaxRankSpellByExist(player, 15286);
    PriestIDLE_AllShadowRes = BotUtility::FindMaxRankSpellByExist(player, 48170);
    PriestIDLE_ShadowRes = BotUtility::FindMaxRankSpellByExist(player, 48169);
    PriestIDLE_ShadowStatus = BotUtility::FindMaxRankSpellByExist(player, 15473);
    PriestIDLE_Revive = BotUtility::FindMaxRankSpellByExist(player, 48171);

    PriestGuard_ShadowFear = BotUtility::FindMaxRankSpellByExist(player, 64044);
    PriestGuard_AOEFear = BotUtility::FindMaxRankSpellByExist(player, 10890);
    PriestGuard_DefFear = BotUtility::FindMaxRankSpellByExist(player, 6346);
    PriestGuard_RecoverMana = BotUtility::FindMaxRankSpellByExist(player, 47585);
    PriestGuard_DmgAnnul = BotUtility::FindMaxRankSpellByExist(player, 33206);
    PriestGuard_DefShield = BotUtility::FindMaxRankSpellByExist(player, 48066);
    PriestGuard_SelfHealth = BotUtility::FindMaxRankSpellByExist(player, 48173);
    PriestGuard_GuardSoul = BotUtility::FindMaxRankSpellByExist(player, 47788);

    PriestAssist_SoulAbs = BotUtility::FindMaxRankSpellByExist(player, 14751);
    PriestAssist_AddHolyPower = BotUtility::FindMaxRankSpellByExist(player, 10060);
    PriestAssist_AllDispel = BotUtility::FindMaxRankSpellByExist(player, 32375);
    PriestAssist_Dispel = BotUtility::FindMaxRankSpellByExist(player, 988);
    PriestAssist_ShadowDemon = BotUtility::FindMaxRankSpellByExist(player, 34433);
    PriestAssist_Silence = BotUtility::FindMaxRankSpellByExist(player, 15487);
    PriestAssist_AllResMana = BotUtility::FindMaxRankSpellByExist(player, 64901);
    PriestAssist_AllResLife = BotUtility::FindMaxRankSpellByExist(player, 64843);
    PriestAssist_DecIllness = BotUtility::FindMaxRankSpellByExist(player, 552);

    PriestDebuf_Ache = BotUtility::FindMaxRankSpellByExist(player, 48125);
    PriestDebuf_Drown = BotUtility::FindMaxRankSpellByExist(player, 48158);
    PriestDebuf_Plague = BotUtility::FindMaxRankSpellByExist(player, 48300);
    PriestAOE_ShadowExplode = BotUtility::FindMaxRankSpellByExist(player, 53023);
    PriestAOE_HolyNova = BotUtility::FindMaxRankSpellByExist(player, 48078);

    PriestShadow_ShadowTouch = BotUtility::FindMaxRankSpellByExist(player, 48160);
    PriestShadow_Knocking = BotUtility::FindMaxRankSpellByExist(player, 48127);
    PriestShadow_Lech = BotUtility::FindMaxRankSpellByExist(player, 48156);
    PriestHoly_Smite = BotUtility::FindMaxRankSpellByExist(player, 48123);
    PriestHoly_BigFire = BotUtility::FindMaxRankSpellByExist(player, 48135);
    PriestPrecept_ManaBurn = BotUtility::FindMaxRankSpellByExist(player, 8129);

    PriestHeal_ZeroHeal = BotUtility::FindMaxRankSpellByExist(player, 2050);
    PriestHeal_LowHeal = BotUtility::FindMaxRankSpellByExist(player, 6064);
    PriestHeal_Resume = BotUtility::FindMaxRankSpellByExist(player, 48068);
    PriestHeal_FastHeal = BotUtility::FindMaxRankSpellByExist(player, 48071);
    PriestHeal_BigHeal = BotUtility::FindMaxRankSpellByExist(player, 48063);
    PriestHeal_LinkHeal = BotUtility::FindMaxRankSpellByExist(player, 48120);
    PriestHeal_UnionHeal = BotUtility::FindMaxRankSpellByExist(player, 48113);
    PriestHeal_RingHeal = BotUtility::FindMaxRankSpellByExist(player, 48089);
    PriestHeal_AOEHeal = BotUtility::FindMaxRankSpellByExist(player, 48072);
    PriestHeal_Awareness = BotUtility::FindMaxRankSpellByExist(player, 53007);

    PriestFlag_DeadSoul = 27827;
    PriestFlag_NonShield = 6788;
}
