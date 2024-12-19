#pragma once
#include "GameFieldView.h"
#include "SkillManagerView.h"
#include "game/GameStateDTO.h"

class BattleView {
private:
    GameFieldView playerField;
    GameFieldView enemyField;
    SkillManagerView skillManager;
public:
    explicit BattleView(GameStateDTO* matchDto);
    void printBattleState();
};
