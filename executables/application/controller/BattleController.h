#pragma once

#include "../model/ComputerPlayer.h"
#include "../model/Player.h"
#include "../view/GameFieldView.h"
#include "../view/SkillManagerView.h"
#include "save/GameSaveCreator.h"

namespace cpp_warships::application {

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
        void applySkill(input_parser::ParsedOptions options);
        void battle(input_parser::ParsedOptions options);
        explicit BattleController(GameStateDTO* dto);
        ~BattleController();
        BattleWinner getBattleWinner() const;
    };
} // namespace cpp_warships::application