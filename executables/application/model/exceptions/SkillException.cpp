#include "SkillException.h"

#include <cpp_warships/utilities/include/ViewHelper.h>

namespace cpp_warships::application {

    SkillException::SkillException(const std::string& msg) : message(msg) {}

    void SkillException::displayError() const { ViewHelper::errorOut("Skill Error: " + message); }
} // namespace cpp_warships::application
