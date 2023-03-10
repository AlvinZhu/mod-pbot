#ifndef _BOT_ARENA_AI_
#define _BOT_ARENA_AI_

#include "PlayerBotAI.h"
#include "BotAITool.h"
#include "BotAISpells.h"
#include "SpellMgr.h"
#include "GridNotifiers.h"
#include <mutex>

class Player;

class BotArenaAI : public PlayerBotAI
{
public:
    static BotArenaAI* CreateBotArenaAIByPlayerClass(Player* player);
    static BotArenaAI* debugArenaAI;

public:
    BotArenaAI(Player* player);
    ~BotArenaAI();

    Player* GetAIPayer() { return me; }
    void UpdateBotAI(uint32 diff) override;
    virtual void ResetBotAI();
    void SetResetAI() { m_HasReset = false; }
    bool IsBGSchedule();

protected:
    int32 m_UpdateTick;

    BotAIHorrorState* pHorrorState;
    BotAIStoped m_CheckStoped;
    BotAIFlee m_Flee;

    bool m_HasReset;

    bool m_IsMeleeBot;
    bool m_IsRangeBot;
    bool m_IsHealerBot;

    std::mutex m_ItemLock;
};

#endif // _BOT_ARENA_AI_
