#include <game_tui/screens/BattleState.h>

#include <algorithm>

namespace cpp_warships::game_tui {
    int furthestLogScroll(int entryCount) {
        return std::max(0, entryCount - LOG_VISIBLE_LINES);
    }
} // namespace cpp_warships::game_tui
