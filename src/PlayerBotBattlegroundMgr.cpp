#include "PlayerBotBattlegroundMgr.h"
#include "Player.h"
#include "CommandAB.h"
#include "CommandWS.h"
#include "CommandEY.h"
#include "CommandAV.h"
#include "CommandIC.h"

PlayerBotBattlegroundMgr* PlayerBotBattlegroundMgr::instance()
{
    static PlayerBotBattlegroundMgr instance;
    return &instance;
}

void BotBattlegroundData::AddPlayerBot(Player* player)
{
    if (player->GetTeamId() == TEAM_ALLIANCE && AllianceCommander)
        AllianceCommander->AddPlayerBot(player);
    else if (player->GetTeamId() == TEAM_HORDE && HordeCommander)
        HordeCommander->AddPlayerBot(player);
}

void BotBattlegroundData::RemovePlayerBot(Player* player)
{
    if (player->GetTeamId() == TEAM_ALLIANCE && AllianceCommander)
        AllianceCommander->RemovePlayerBot(player);
    else if (player->GetTeamId() == TEAM_HORDE && HordeCommander)
        HordeCommander->RemovePlayerBot(player);
}

void BotBattlegroundData::Update(uint32 diff)
{
    if (AllianceCommander)
        AllianceCommander->Update(diff);
    if (HordeCommander)
        HordeCommander->Update(diff);
}

void BotBattlegroundData::InsureCommander(Battleground* bg, BattlegroundTypeId bgType)
{
    if (!AllianceCommander)
    {
        switch (bgType)
        {
        case BATTLEGROUND_AB:
            AllianceCommander = new CommandAB(bg, TeamId::TEAM_ALLIANCE);
            break;
        case BATTLEGROUND_WS:
            AllianceCommander = new CommandWS(bg, TeamId::TEAM_ALLIANCE);
            break;
        case BATTLEGROUND_EY:
            AllianceCommander = new CommandEY(bg, TeamId::TEAM_ALLIANCE);
            break;
        case BATTLEGROUND_AV:
            AllianceCommander = new CommandAV(bg, TeamId::TEAM_ALLIANCE);
            break;
#ifndef TBC243
        case BATTLEGROUND_IC:
            AllianceCommander = new CommandIC(bg, TeamId::TEAM_ALLIANCE);
            break;
#endif
        }
    }
    if (!HordeCommander)
    {
        switch (bgType)
        {
        case BATTLEGROUND_AB:
            HordeCommander = new CommandAB(bg, TeamId::TEAM_HORDE);
            break;
        case BATTLEGROUND_WS:
            HordeCommander = new CommandWS(bg, TeamId::TEAM_HORDE);
            break;
        case BATTLEGROUND_EY:
            HordeCommander = new CommandEY(bg, TeamId::TEAM_HORDE);
            break;
        case BATTLEGROUND_AV:
            HordeCommander = new CommandAV(bg, TeamId::TEAM_HORDE);
            break;
#ifndef TBC243
        case BATTLEGROUND_IC:
            HordeCommander = new CommandIC(bg, TeamId::TEAM_HORDE);
            break;
#endif
        }
    }
}

void BotBattlegroundData::InitCommander()
{
    if (AllianceCommander)
        AllianceCommander->Initialize();
    if (HordeCommander)
        HordeCommander->Initialize();
}

void BotBattlegroundData::ResetCommander(Battleground* bg)
{
    if (AllianceCommander)
        AllianceCommander->UpdateBelongBattleground(bg);
    if (HordeCommander)
        HordeCommander->UpdateBelongBattleground(bg);
}

void BotBattlegroundData::ReadyCommander()
{
    if (AllianceCommander)
        AllianceCommander->ReadyGame();
    if (HordeCommander)
        HordeCommander->ReadyGame();
}

void BotBattlegroundData::StartCommander()
{
    if (AllianceCommander)
        AllianceCommander->StartGame();
    if (HordeCommander)
        HordeCommander->StartGame();
}

CommandBG* BotBattlegroundData::GetCommander(TeamId team)
{
    if (team == TEAM_ALLIANCE)
        return AllianceCommander;
    else if (team == TEAM_HORDE)
        return HordeCommander;
    return nullptr;
}

void PlayerBotBattlegroundMgr::PlayerEnter(BattlegroundMap* map, Player* player)
{
    BotBattlegroundData *mapBgInfo = map->CustomData.Get<BotBattlegroundData>("BotBGInfo");
    if (!mapBgInfo)
        return;

    mapBgInfo->RemovePlayerBot(player);
}

void PlayerBotBattlegroundMgr::PlayerLeave(BattlegroundMap* map, Player* player)
{
    BotBattlegroundData *mapBgInfo = map->CustomData.Get<BotBattlegroundData>("BotBGInfo");
    if (!mapBgInfo)
        return;

    mapBgInfo->RemovePlayerBot(player);
}

void PlayerBotBattlegroundMgr::Update(BattlegroundMap* map, uint32 diff)
{
    BotBattlegroundData *mapBgInfo = map->CustomData.Get<BotBattlegroundData>("BotBGInfo");
    if (!mapBgInfo)
        return;

    mapBgInfo->Update(diff);
}

void PlayerBotBattlegroundMgr::InsureCommander(Map* map, BattlegroundTypeId bgType)
{
    if (!map || !map->IsBattleground())
        return;

    BotBattlegroundData *mapBgInfo = map->CustomData.GetDefault<BotBattlegroundData>("BotBGInfo");
    mapBgInfo->InsureCommander(((BattlegroundMap*)map)->GetBG(), bgType);
}

void PlayerBotBattlegroundMgr::InitCommander(Map* map)
{
    if (!map)
        return;

    BotBattlegroundData *mapBgInfo = map->CustomData.Get<BotBattlegroundData>("BotBGInfo");
    if (!mapBgInfo)
        return;

    mapBgInfo->InitCommander();
}

void PlayerBotBattlegroundMgr::ResetCommander(Map* map)
{
    if (!map || !map->IsBattleground())
        return;

    BotBattlegroundData *mapBgInfo = map->CustomData.Get<BotBattlegroundData>("BotBGInfo");
    if (!mapBgInfo)
        return;

    mapBgInfo->ResetCommander(((BattlegroundMap*)map)->GetBG());
}

void PlayerBotBattlegroundMgr::ReadyCommander(Map* map)
{
    if (!map)
        return;

    BotBattlegroundData *mapBgInfo = map->CustomData.Get<BotBattlegroundData>("BotBGInfo");
    if (!mapBgInfo)
        return;

    mapBgInfo->ReadyCommander();
}

void PlayerBotBattlegroundMgr::StartCommander(Map* map)
{
    if (!map)
        return;

    BotBattlegroundData *mapBgInfo = map->CustomData.Get<BotBattlegroundData>("BotBGInfo");
    if (!mapBgInfo)
        return;

    mapBgInfo->StartCommander();
}

CommandBG* PlayerBotBattlegroundMgr::GetCommander(Map* map, TeamId team)
{
    if (!map)
        return nullptr;

    BotBattlegroundData *mapBgInfo = map->CustomData.Get<BotBattlegroundData>("BotBGInfo");
    if (!mapBgInfo)
        return nullptr;

    return mapBgInfo->GetCommander(team);
}

CommandBG* PlayerBotBattlegroundMgr::GetCommander(Battleground* bg, TeamId team)
{
    if (!bg)
        return nullptr;

    return GetCommander(bg->GetBgMap(), team);
}
