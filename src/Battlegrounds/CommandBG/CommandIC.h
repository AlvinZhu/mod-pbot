
#ifndef _COMMAND_IC_H_
#define _COMMAND_IC_H_

#include "CommandBG.h"
#include "BattlegroundIC.h"

#define AIWP_IC_SELF_READY 0
#define AIWP_IC_SELF_CITY 1
#define AIWP_IC_SELF_CITYTELE 2
#define AIWP_IC_SELF_CITYOUT 3
#define AIWP_IC_WORKSHOP 4
#define AIWP_IC_DOCK 5
#define AIWP_IC_AIRSHIP 6
#define AIWP_IC_ENEMY_CITYOUT 7
#define AIWP_IC_ENEMY_CITYTELE 8
#define AIWP_IC_ENEMY_CITY 9

#define AIWP_IC_GROUPFOCUS 10
#define AIWP_IC_ENEMY_GROUPFOCUS 11

#define MAX_ICAIWP_COUNT 12

class CommandIC : public CommandBG
{
public:
    CommandIC(Battleground* pBG, TeamId team);
    ~CommandIC();

    void Initialize() override;
    const Creature* GetMatchGraveyardNPC(const Player* player) override;
    void Update(uint32 diff) override;
    AIWaypoint* GetReadyPosition() override;

private:
    void RndStartCommand();
    void ProcessRegulation();

private:
    void ProcessICNodeRequirement(uint32 nodeType, AIWaypoint* waypoint, uint32 baseCount, PlayerGUIDs& players);
    PlayerGUIDs GetICFlagRangePlayerByTeam(uint32 nodeType, TeamId team, float range = 40);
    bool AcceptCommandByPlayerGUID(uint64 guid, AIWaypoint* targetAIWP, bool isFlag = false);
    bool AcceptCommandByPlayerGUID(uint64 guid, ObjectGuid flagGuid, bool isFlag = false);
    void TryOccupiedICNode(uint64 guid);
    uint32 GetICNodeObjectType(uint32 index, TeamId team);

private:
    int32 lastUpdateTick;
};

#endif // !_COMMAND_IC_H_
