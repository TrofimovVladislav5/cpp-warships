#pragma once

#include "Skill.h"
#include "../game/MatchSettings.h"

namespace cpp_warships::application {

    class DoubleDamage : public ISkill {
    private:
        MatchSettings* settings;

    public:
        explicit DoubleDamage(MatchSettings* settings);
        void apply() override;
    };
} // namespace cpp_warships::application