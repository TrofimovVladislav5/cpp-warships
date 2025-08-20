#pragma once

#include "../GameFieldView.h"
#include "../MatchSettings.h"
#include "Skill.h"

namespace cpp_warships::application {

    class Scanner : public ISkill {
    private:
        GameField* opponentField;

    public:
        explicit Scanner(GameField* opponentField);
        void apply() override;
    };
} // namespace cpp_warships::application