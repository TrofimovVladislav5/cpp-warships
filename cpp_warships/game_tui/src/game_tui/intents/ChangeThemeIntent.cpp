#include <game_tui/intents/ChangeThemeIntent.h>

#include <string>
#include <utility>

#include <game_tui/intents/IntentContext.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::game_tui {
    ChangeThemeIntent::ChangeThemeIntent(std::string themeName)
        : themeName_(std::move(themeName)) {}

    void ChangeThemeIntent::applyTo(const IntentContext& context) const {
        context.session.changeTheme(themeName_);
    }
} // namespace cpp_warships::game_tui
