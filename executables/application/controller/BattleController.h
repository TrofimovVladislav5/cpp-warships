#pragma once
#include "../model/ComputerPlayer.h"
#include "../model/Player.h"
#include "../view/GameFieldView.h"
#include "../view/SkillManagerView.h"
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
    void applySkill(cpp_warships::input_parser::ParsedOptions options);
    void battle(cpp_warships::input_parser::ParsedOptions options);
    explicit BattleController(GameStateDTO* dto);
    ~BattleController();
    BattleWinner getBattleWinner() const;
};
