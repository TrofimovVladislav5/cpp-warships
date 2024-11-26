#pragma once
#include "SkillManagerView.h"
#include "../model/ComputerPlayer.h"
#include "../view/GameFieldView.h"
#include "../model/Player.h"


class BattleController {
private:
    GameFieldView* playerView;
    GameFieldView* opponentView;
    SkillManagerView* skillManagerView;
    Player* player;
    ComputerPlayer* computer;
    bool battleIsFinished;
    void printBattleState();
public:
    explicit BattleController(GameStateDTO* dto);
    ~BattleController();
    bool finishBattle() const;
    void applySkill(ParsedOptions options);
    void battle(ParsedOptions options);
};