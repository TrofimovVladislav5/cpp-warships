#include "SkillException.h"

#include "ViewHelper.h"

SkillException::SkillException(const std::string& msg) : message(msg) {}

void SkillException::displayError() const { ViewHelper::errorOut("Skill Error: " + message); }
