#pragma once

namespace cpp_warships::model {
    /** @brief When a handler is listening. A handler subscribed to Always hears everything;
     *  the rest hear only what arrives while the player is in that part of the game.
     *  Scopes are what let one router stand in for a router per screen. */
    enum class EventScope {
        Always,
        Menu,
        Placement,
        Battle,
    };
} // namespace cpp_warships::model
