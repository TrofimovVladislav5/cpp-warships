#pragma once

namespace cpp_warships::head {
    /** @brief The screens the application can be showing. A finished match is
     * still the battle screen, which says so: there is nothing a separate one
     * would add.
     */
    enum class ScreenKind {
        Menu,
        Placement,
        Battle,
    };
}  // namespace cpp_warships::head
