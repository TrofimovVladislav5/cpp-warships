#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/SaveBrowserInput.h>
#include <application/head/common/input/keys/LeaveToMenuKey.h>
#include <application/head/common/input/keys/SaveBrowserKeys.h>

#include <memory>

namespace cpp_warships::head::common::input {
    SaveBrowserInput::SaveBrowserInput(PresentationContext& context) {
        bind(std::make_unique<keys::MoveSaveSelectionKey>(context));
        bind(std::make_unique<keys::LoadSelectedSaveKey>(context));
        bind(std::make_unique<keys::DeleteSelectedSaveKey>(context));
        bind(std::make_unique<keys::LeaveToMenuKey>());
    }
}  // namespace cpp_warships::head::common::input
