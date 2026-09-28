#pragma once

#include <string>
#include <vector>

namespace cpp_warships::model {
    class ApplicationContext;
}

namespace cpp_warships::head::common::render {
    /** @brief How many of the newest notices are worth the room to show. */
    inline constexpr int NOTICES_SHOWN = 2;

    /** @brief The newest notices, newest last, at most NOTICES_SHOWN of them. */
    [[nodiscard]] std::vector<std::string> noticesToShow(
        const model::ApplicationContext& application
    );
}  // namespace cpp_warships::head::common::render
