#pragma once
#include "../model/ComputerPlayer.h"
#include "../model/Player.h"
#include "../view/GameFieldView.h"
#include "SkillManagerView.h"
#include "save/GameSaveCreator.h"

enum BattleWinner { User, Computer, None };

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
    void applySkill(ParsedOptions options);
    void battle(ParsedOptions options);
    explicit BattleController(GameStateDTO* dto);
    ~BattleController();
    BattleWinner getBattleWinner() const;
};
