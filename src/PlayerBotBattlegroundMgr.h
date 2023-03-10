#ifndef __PLAYERBOTBATTLEGROUNDMGR_H__
#define __PLAYERBOTBATTLEGROUNDMGR_H__

#include "Log.h"
#include "Common.h"
#include "SharedDefines.h"
#include "DatabaseEnv.h"

class Player;
class CommandBG;

class BotBattlegroundData : public DataMap::Base
{
public:
    BotBattlegroundData() : AllianceCommander(nullptr), HordeCommander(nullptr) {}

    void AddPlayerBot(Player* player);
    void RemovePlayerBot(Player* player);
    void Update(uint32 diff);

    void InsureCommander(Battleground* bg, BattlegroundTypeId bgType);
    void InitCommander();
    void ResetCommander(Battleground* bg);
    void ReadyCommander();
    void StartCommander();
    CommandBG* GetCommander(TeamId team);

private:
    CommandBG* AllianceCommander;
    CommandBG* HordeCommander;
};

class TC_GAME_API PlayerBotBattlegroundMgr
{
public:
    static PlayerBotBattlegroundMgr* instance();

    void PlayerEnter(BattlegroundMap* map, Player* player);
    void PlayerLeave(BattlegroundMap* map, Player* player);
    void Update(BattlegroundMap* map, uint32 diff);

    void InsureCommander(Map* map, BattlegroundTypeId bgType);
    void InitCommander(Map* map);
    void ResetCommander(Map* map);
    void ReadyCommander(Map* map);
    void StartCommander(Map* map);
    CommandBG* GetCommander(Map* map, TeamId team);
    CommandBG* GetCommander(Battleground* bg, TeamId team);
};

#define sPlayerBotBGMgr PlayerBotBattlegroundMgr::instance()

#endif // __PLAYERBOTBATTLEGROUNDMGR_H__
