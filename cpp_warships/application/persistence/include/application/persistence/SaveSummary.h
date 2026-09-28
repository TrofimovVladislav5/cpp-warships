#pragma once

#include <string>

namespace cpp_warships::persistence {
    /** @brief A save as the player sees it: when it was made, and what they called it. */
    struct SaveSummary {
        std::string id;
        std::string name;
    };
}  // namespace cpp_warships::persistence
