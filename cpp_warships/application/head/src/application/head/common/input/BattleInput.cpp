#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/BattleInput.h>
#include <application/head/common/input/keys/BattleKeys.h>
#include <application/head/common/input/keys/LeaveToMenuKey.h>

#include <memory>

namespace cpp_warships::head::common::input {
    BattleInput::BattleInput(PresentationContext& context) {
        bind(std::make_unique<keys::ScrollLogWithWheelKey>(context));
        bind(std::make_unique<keys::FireWithPointerKey>(context));
        bind(std::make_unique<keys::AimWithPointerAtEnemyKey>(context));
        bind(std::make_unique<keys::MoveBattleAimKey>(context));
        bind(std::make_unique<keys::ScrollLogBackKey>(context));
        bind(std::make_unique<keys::ScrollLogForwardKey>(context));
        bind(std::make_unique<keys::FireKey>(context));
        bind(std::make_unique<keys::UseSkillKey>(context));
        bind(std::make_unique<keys::LeaveToMenuKey>());
    }
}  // namespace cpp_warships::head::common::input
