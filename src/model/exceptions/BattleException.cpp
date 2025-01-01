#include "BattleException.h"

#include "ViewHelper.h"

BattleException::BattleException(const std::string& msg) : message(msg) {}

void BattleException::displayError() const { ViewHelper::errorOut("Battle Error: " + message); }
