#pragma once

#include <string>

namespace cpp_warships::head::common::state {
    /** @brief What the player has typed as a name for the match they are putting away. */
    struct SaveNamingState {
        std::string typedName;
    };
}  // namespace cpp_warships::head::common::state
