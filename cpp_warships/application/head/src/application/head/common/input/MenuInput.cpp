#include <application/head/common/PresentationContext.h>
#include <application/head/common/input/MenuInput.h>
#include <application/head/common/input/keys/MenuKeys.h>

#include <memory>

namespace cpp_warships::head::common::input {
    MenuInput::MenuInput(PresentationContext& context) {
        bind(std::make_unique<keys::ResizeBoardKey>(context));
        bind(std::make_unique<keys::CycleThemeKey>(context));
        bind(std::make_unique<keys::StartMatchKey>(context));
        bind(std::make_unique<keys::ResumeMatchKey>());
        bind(std::make_unique<keys::LoadMatchKey>());
        bind(std::make_unique<keys::SaveAndQuitKey>());
        bind(std::make_unique<keys::QuitKey>());
    }
}  // namespace cpp_warships::head::common::input
