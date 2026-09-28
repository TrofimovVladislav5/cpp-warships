#pragma once

#include <string>

namespace cpp_warships::persistence {
    /** @brief A save as the player sees it: what identifies it, when it was
     * written and what they called it. */
    struct SaveSummary {
        std::string id;
        std::string name;
        std::string timestamp;
    };
}  // namespace cpp_warships::persistence
