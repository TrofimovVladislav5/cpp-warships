#pragma once

#include "../../view/GameFieldView.h"
#include "../game/MatchSettings.h"
#include "Skill.h"

class Scanner : public ISkill {
private:
    GameField* opponentField;
public:
    explicit Scanner(GameField* opponentField);
    void apply() override;
};
