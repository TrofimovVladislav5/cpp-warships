#pragma once

#include <application/head/common/Theme.h>
#include <application/model/ApplicationContext.h>

#include <ftxui/dom/elements.hpp>

namespace cpp_warships::head {
    /** @brief The newest notices as a block to sit under a screen, or nothing
     * at all when there is nothing to say. */
    [[nodiscard]] ftxui::Element
    noticeBlock(const Theme& theme, const model::ApplicationContext& application);
}  // namespace cpp_warships::head
