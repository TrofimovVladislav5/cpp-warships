#pragma once

#include <application/head/common/input/BoundScreenInput.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input {
    /** @brief Typing a name for the match being put away. Nothing is saved until a name
     * has been given, so the game hears nothing until then. */
    class SaveNamingInput final : public BoundScreenInput {
    public:
        explicit SaveNamingInput(PresentationContext& context);
    };
}  // namespace cpp_warships::head::common::input
