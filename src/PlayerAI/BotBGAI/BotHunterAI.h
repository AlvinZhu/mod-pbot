
#ifndef _BOT_HUNTER_AI_H
#define _BOT_HUNTER_AI_H

#include "ScriptSystem.h"
#include "BotAI.h"
#include "AIWaypointsMgr.h"

enum HunterTalentType
{
    HunterTalent_Wild,
    HunterTalent_Shot,
    HunterTalent_Survival
};

class BotHunterAI : public BotBGAI
{
public:
    BotHunterAI(Player* player) :
        BotBGAI(player),
        m_BotTalentType(0),
        m_IsSupplemented(false),
        m_IsReviveManaModel(false)
    {}
    ~BotHunterAI() {}

    void ResetBotAI() override;

protected:
    uint32 GetManaPowerPer();
    void InitializeSpells();
    void UpdateTalentType();
    bool NeedFlee() override;
    void ProcessReady() override;
    void ProcessFlee() override;
    bool ProcessNormalSpell() override;
    void ProcessMeleeSpell(Unit* pTarget) override;
    void ProcessRangeSpell(Unit* pTarget) override;
    bool ProcessAura(bool isFlee);

    //bool TryStartControlCommand() override;
    //float TryPushControlCommand(Player* pTarget) override;
    bool CastRangeSpell(Unit* pTarget);
    bool CastMeleeSpell(Unit* pTarget);
    void PetAction(Pet* pPet, Unit* pTarget);
    bool HasRootMechanic();
    bool TargetIsSuppress(Unit* pTarget);
    bool CheckManaModel();

private:
    uint32 m_BotTalentType;
    bool m_IsSupplemented;
    bool m_IsReviveManaModel;

    uint32 HunterIDLE_SummonPet;// 883			�ٻ�����
    uint32 HunterIDLE_RevivePet;// 982			�������
    uint32 HunterIDLE_ManaAura;// 34074			���������ػ�
    uint32 HunterIDLE_DodgeAura;// 13163		�����ػ�
    uint32 HunterIDLE_EagleAura;// 27044		����ػ�
    uint32 HunterIDLE_DragonAura;// 61847		��ӥ�ػ� ����������ػ�
    uint32 HunterIDLE_ShotAura;// 19506			ǿ���⻷(���ϵ)

    uint32 HunterTrap_FarFrozen;// 60192		Զ�̱�������
    uint32 HunterTrap_Frozen;// 14311			��������
    uint32 HunterTrap_Ice;// 13809				��˪����
    uint32 HunterTrap_Viper;// 34600			��������
    uint32 HunterTrap_Explode;// 49067			��ը����
    uint32 HunterTrap_Fire;// 49056				��������
    uint32 HunterTrap_Shot;// 63672				���Ǽ�(����ϵ)

    uint32 HunterAssist_ClearRoot;// 53271		�Ƴ�����
    uint32 HunterAssist_PetCommand;// 34026		����ɱ��
    uint32 HunterAssist_HealPet;// 48990		��������
    uint32 HunterAssist_PetStun;// 19577		�������Ŀ��(Ұ��ϵ)
    uint32 HunterAssist_PetRage;// 19574		������Լ���(Ұ��ϵ)
    uint32 HunterAssist_Stamp;// 53338			����ӡ��
    uint32 HunterAssist_FalseDead;// 5384		����
    uint32 HunterAssist_BackJump;// 781			����
    uint32 HunterAssist_FastSpeed;// 3045		�������BUF
    uint32 HunterAssist_ReadyCD;// 23989		׼������CD(���ϵ)

    uint32 HunterMelee_BackRoot;// 48999		�мܺ󷴻�����(����ϵ)
    uint32 HunterMelee_NoDamage;// 19263		���� �޷�����
    uint32 HunterMelee_DecSpeed;// 2974			ˤ�� ��ս���ƶ���
    uint32 HunterMelee_NextAtt;// 48996			next��ս������ǿ
    uint32 HunterMelee_MeleeAtt;// 53339		��ս����

    uint32 HunterDebug_Damage;// 49001			�˺�����
    uint32 HunterDebug_Mana;// 3034				��������
    uint32 HunterDebug_Sleep;// 49012			��˯����(����ϵ)

    uint32 HunterShot_AOEShot;// 58434			AOE���
    uint32 HunterShot_CharmShot;// 19503		������(����ϵ)
    uint32 HunterShot_Explode;// 60053			��ը���(����ϵ)
    uint32 HunterShot_Aim;// 49050				��׼���(���ϵ)
    uint32 HunterShot_Silence;// 34490			��Ĭ���(���ϵ)
    uint32 HunterShot_Shock;// 5116				�����
    uint32 HunterShot_Cast;// 49052				�ȹ���� ʩ��ʱ������
    uint32 HunterShot_MgcShot;// 49045			�������
    uint32 HunterShot_KillShot;// 61006			��ɱ��� Ŀ��20%����Ѫնɱʽ���
    uint32 HunterShot_MulShot;// 49048			�������
    uint32 HunterShot_QMLShot;// 53209			���������(���ϵ)
};

#endif // !_BOT_HUNTER_AI_H
