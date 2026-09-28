#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/SaveNamingInput.h>
#include <application/head/common/input/keys/LeaveToMenuKey.h>
#include <application/head/common/input/keys/SaveNamingKeys.h>

#include <memory>

namespace cpp_warships::head::common::input {
    SaveNamingInput::SaveNamingInput(PresentationContext& context) {
        bind(std::make_unique<keys::ConfirmNameKey>(context));
        bind(std::make_unique<keys::EraseNameKey>(context));
        bind(std::make_unique<keys::LeaveToMenuKey>());
        bind(std::make_unique<keys::TypeNameKey>(context));
    }
}  // namespace cpp_warships::head::common::input
