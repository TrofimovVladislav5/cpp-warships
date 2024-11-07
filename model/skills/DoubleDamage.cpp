#include "DoubleDamage.h"
#include <iostream>
#include <set>

#include "../game/MatchSettings.h"

DoubleDamage::DoubleDamage(MatchSettings* settings)
    : settings(settings)
{}

void DoubleDamage::apply() {
    // settings->setDamageCount(2);
    // settings->setActiveDoubleDamage(true);
}