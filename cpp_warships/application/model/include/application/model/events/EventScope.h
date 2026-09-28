#pragma once

namespace cpp_warships::model {
    /** @brief When a handler is listening. */
    enum class EventScope {
        Always,
        Menu,
        Placement,
        Battle,
    };
}  // namespace cpp_warships::model
