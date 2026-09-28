#pragma once

#include <application/head/common/input/BoundScreenInput.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input {
    /** @brief The list of saved games, read in the game's terms. Which one is highlighted
     * is the interface's own business; loading and deleting are the game's. */
    class SaveBrowserInput final : public BoundScreenInput {
    public:
        explicit SaveBrowserInput(PresentationContext& context);
    };
}  // namespace cpp_warships::head::common::input
