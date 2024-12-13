#pragma once
#include "SkillManagerView.h"
#include "../model/ComputerPlayer.h"
#include "../view/GameFieldView.h"
#include "../model/Player.h"
#include "save/GameSaveCreator.h"


class BattleController {
private:
    GameFieldView* playerView;
    GameFieldView* opponentView;
    SkillManagerView* skillManagerView;
    Player* player;
    ComputerPlayer* computer;
    GameSaveCreator* saveCreator;
    bool battleIsFinished;
    void printBattleState();
    std::string command;
public:
    void applySkill(ParsedOptions options);
    void battle(ParsedOptions options);
    void pause(ParsedOptions options);
    explicit BattleController(GameStateDTO* dto);
    ~BattleController();
    bool finishBattle();
    std::string getCommand() const;
};
