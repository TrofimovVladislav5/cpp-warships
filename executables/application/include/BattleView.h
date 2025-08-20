#pragma once

#include "GameFieldView.h"
#include "GameStateDTO.h"
#include "player_skills/SkillManagerView.h"

namespace cpp_warships::application {

    class BattleView {
    private:
        GameFieldView playerField;
        GameFieldView enemyField;
        SkillManagerView skillManager;

    public:
        explicit BattleView(GameStateDTO* matchDto);
        void printBattleState();
    };
} // namespace cpp_warships::application