#include "../../include/player_skills/DoubleDamage.h"

#include <iostream>

#include "../../include/MatchSettings.h"

namespace cpp_warships::application {

    DoubleDamage::DoubleDamage(MatchSettings* settings) : settings(settings) {}

    void DoubleDamage::apply() {
        std::cout << "Toggle double damage skill" << std::endl;
        settings->toggleDoubleDamage(true);
    }
} // namespace cpp_warships::application