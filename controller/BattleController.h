#pragma once
#include "../model/ComputerPlayer.h"
#include "../view/model/GameFieldView.h"
#include "../model/Player.h"


class BattleController {
private:
    MatchSettings settings;
    GameFieldView* playerView;
    GameFieldView* opponentView;
    Player* player;
    ComputerPlayer* computer;
    bool battleStatus();
    bool battleIsFinished;
public:
    explicit BattleController(MatchSettings& settings);
    ~BattleController();
    bool finishBattle() const;
    void applySkill(ParsedOptions options);
    void battle(ParsedOptions options);
};