#pragma once

#include <ftxui/dom/elements.hpp>

#include <application/head/Theme.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    /** @brief The newest notices as a block to sit under a screen, or nothing at all when
     *  there is nothing to say. Drawn in the theme's danger colour, since everything that
     *  reaches here is something that did not come off. */
    [[nodiscard]] ftxui::Element noticeBlock(
            const Theme& theme,
            const model::ApplicationContext& application
    );
} // namespace cpp_warships::head
