#pragma once

#include <string>
#include <vector>

#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    /** @brief How many of the newest notices are worth the room to show. */
    inline constexpr int NOTICES_SHOWN = 2;

    /** @brief The newest notices, newest last, at most NOTICES_SHOWN of them.
     *  This is the whole of how a failure reaches the player: the processor leaves it on
     *  the context, and whatever is drawing reads it off, the same as anything else. */
    [[nodiscard]] std::vector<std::string> noticesToShow(
            const model::ApplicationContext& application
    );
} // namespace cpp_warships::head
