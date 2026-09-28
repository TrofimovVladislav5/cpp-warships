#pragma once

#include <ftxui/dom/elements.hpp>

namespace cpp_warships::head::common {
    struct Theme;
}

namespace cpp_warships::model {
    class ApplicationContext;
}

namespace cpp_warships::head::tui {
    /** @brief The newest notices as a block to sit under a screen, or nothing
     * at all when there is nothing to say. */
    [[nodiscard]] ftxui::Element noticeBlock(
        const common::Theme& theme,
        const model::ApplicationContext& application
    );
}  // namespace cpp_warships::head::tui
