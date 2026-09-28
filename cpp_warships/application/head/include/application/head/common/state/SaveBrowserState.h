#pragma once

namespace cpp_warships::head::common::state {
    /** @brief Which of the saved games the player is looking at. */
    struct SaveBrowserState {
        int selectedIndex = 0;
    };
}  // namespace cpp_warships::head::common::state
