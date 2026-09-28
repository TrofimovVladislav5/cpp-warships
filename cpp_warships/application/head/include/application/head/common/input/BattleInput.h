#pragma once

#include <application/head/common/input/BoundScreenInput.h>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input {
    /** @brief The battle read in the game's terms. Taking aim and reading back through the
     * log never trouble the game; only firing and spending a skill do. */
    class BattleInput final : public BoundScreenInput {
    public:
        explicit BattleInput(PresentationContext& context);
    };
}  // namespace cpp_warships::head::common::input
