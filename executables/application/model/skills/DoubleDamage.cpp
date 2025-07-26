#include "DoubleDamage.h"

#include "../game/MatchSettings.h"

namespace cpp_warships::application {

    DoubleDamage::DoubleDamage(MatchSettings* settings) : settings(settings) {}

    void DoubleDamage::apply() {
        settings->damageCount *= 2;
        settings->isActiveDoubleDamage = true;
    }
} // namespace cpp_warships::application