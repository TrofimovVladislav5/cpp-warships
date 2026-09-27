#include <application/head/screens/BattleState.h>

#include <algorithm>

namespace cpp_warships::head {
    int furthestLogScroll(int entryCount) {
        return std::max(0, entryCount - LOG_VISIBLE_LINES);
    }
} // namespace cpp_warships::head
