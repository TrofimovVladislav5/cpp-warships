#pragma once

namespace cpp_warships::model {
    class ApplicationContext;
}

namespace cpp_warships::head {
    class ScreenNavigator;
    class Shell;
    class ThemeSelection;

    /** @brief Everything an intent is allowed to act on: the game it belongs to, where the
     *  player is, the host showing it and the palette it is shown in. Nothing else is reachable.
     *  Navigation and the palette sit here only until the interface takes them back over. */
    struct IntentContext {
        model::ApplicationContext& application;
        ScreenNavigator& navigator;
        Shell& shell;
        ThemeSelection& theme;
    };
} // namespace cpp_warships::head
