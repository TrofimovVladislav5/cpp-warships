#pragma once

#include "../../view/GameFieldView.h"
#include "../game/MatchSettings.h"
#include "Skill.h"

class Scanner : public ISkill {
private:
    MatchSettings* settings;
    GameFieldView* scannerView;
public:
    explicit Scanner(MatchSettings* settings);
    void apply() override;
};
