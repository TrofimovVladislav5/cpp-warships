#pragma once

#include <application/head/common/input/BoundScreenInput.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input {
    /** @brief The menu read in the game's terms. Board size and palette are settled here,
     * because neither is the game's business until a match is actually asked for. */
    class MenuInput final : public BoundScreenInput {
    public:
        explicit MenuInput(PresentationContext& context);
    };
}  // namespace cpp_warships::head::common::input
