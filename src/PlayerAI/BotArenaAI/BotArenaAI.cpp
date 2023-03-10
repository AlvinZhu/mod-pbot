#include "BotArenaAI.h"
#include "MoveSplineInit.h"
#include "PlayerBotSetting.h"
#include "WorldSession.h"
#include "Spell.h"
#include "Pet.h"
#include "Group.h"
#include "WorldSession.h"

BotArenaAI* BotArenaAI::debugArenaAI = nullptr;

BotArenaAI* BotArenaAI::CreateBotArenaAIByPlayerClass(Player* player)
{
    PlayerBotSetting::ClearUnknowMount(player);
    BotArenaAI* pAI = nullptr;
    if (!pAI)
        pAI = new BotArenaAI(player);
    pAI->ResetBotAI();
    return pAI;
}

BotArenaAI::BotArenaAI(Player* player) :
    PlayerBotAI(player),
    m_UpdateTick(BOTAI_UPDATE_TICK),
    pHorrorState(nullptr),
    m_CheckStoped(player),
    m_Flee(player),
    m_HasReset(false)
{
    if (!me->IsPvP())
    {
        BotUtility::PlayerBotTogglePVP(player, true);
    }
}

BotArenaAI::~BotArenaAI()
{

}

void BotArenaAI::UpdateBotAI(uint32 diff)
{
    m_UpdateTick -= diff;
    if (m_UpdateTick > 0)
        return;
    m_UpdateTick = BOTAI_UPDATE_TICK;

    //if (!me->IsSettingFinish())
    //	return;
    UpdateTeleport(BOTAI_UPDATE_TICK);
    if (!m_Teleporting.CanMovement())
        return;
    me->UpdateObjectVisibility(false);

    if (!m_HasReset)
        ResetBotAI();

    if (Group* pGroup = me->GetGroup())
    {
        if (me->InArena() || pGroup->isBGGroup())
        {
            int a = 0;
        }
    }
}

void BotArenaAI::ResetBotAI()
{
    PlayerBotSetting::ClearUnknowMount(me);
    m_Movement.ClearMovement();
    m_HasReset = true;
}

bool BotArenaAI::IsBGSchedule()
{
    if (/*me->GetSession()->HasBGSchedule() || */me->InBattleground() || me->isUsingLfg())
        return true;
    return false;
}
