#include "../../include/exceptions/BattleException.h"

#include <cpp_warships/utilities/include/ViewHelper.h>

namespace cpp_warships::application {

    BattleException::BattleException(const std::string& msg) : message(msg) {}

    void BattleException::displayError() const {
        ViewHelper::errorOut("Battle Error: " + message);
    }
} // namespace cpp_warships::application