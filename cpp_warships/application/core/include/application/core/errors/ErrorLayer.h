#pragma once

#include <string_view>

namespace cpp_warships::core::errors {
    /** @brief Which layer of the game an error came from. Every error carries one, so a layer's
     * entry point can tell at a glance whether the error is its own to answer for. */
    enum class ErrorLayer {
        Core,
        Flow,
        Persistence,
        Model,
        Presentation,
    };

    /** @brief What @p layer is called, for the front of an error message. */
    [[nodiscard]] std::string_view nameOf(ErrorLayer layer) noexcept;
}  // namespace cpp_warships::core::errors
