#include <application/head/common/state/BattleState.h>

#include <algorithm>

namespace cpp_warships::head::common::state {
    int furthestLogScroll(int entryCount) {
        return std::max(0, entryCount - LOG_VISIBLE_LINES);
    }
}  // namespace cpp_warships::head::common::state
