#include "DoubleDamage.h"

#include "../game/MatchSettings.h"

DoubleDamage::DoubleDamage(MatchSettings* settings)
    : settings(settings)
{}

void DoubleDamage::apply() {
    settings->damageCount *= 2;
    settings->isActiveDoubleDamage = true;
}