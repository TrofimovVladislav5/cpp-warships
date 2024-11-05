#pragma once
#include "ComputerPlayer.h"
#include "GameFieldView.h"
#include "model/StateContext.h"
#include "model/Player.h"

class BattleController {
private:
    StateContext context;
    GameFieldView* playerView;
    GameFieldView* opponentView;
    Player* player;
    ComputerPlayer* computer;
    bool battleStatus();
    bool battleIsFinished;
public:
    explicit BattleController(StateContext& context);
    ~BattleController();
    bool finishBattle() const;
    void applySkill(ParsedOptions options);
    void battle(ParsedOptions options);
};