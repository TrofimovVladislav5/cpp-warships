#pragma once

namespace cpp_warships::head {
    class Application;
    class ScreenNavigator;
    class Shell;

    /** @brief Everything an intent is allowed to act on: the session it belongs to, where
     *  the player is, and the host showing it all. Nothing else is reachable. */
    struct IntentContext {
        Application& session;
        ScreenNavigator& navigator;
        Shell& shell;
    };
} // namespace cpp_warships::head
