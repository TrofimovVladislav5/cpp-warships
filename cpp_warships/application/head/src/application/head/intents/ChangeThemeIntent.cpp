#include <application/head/intents/ChangeThemeIntent.h>

#include <string>
#include <utility>

#include <application/head/intents/IntentContext.h>
#include <application/head/session/Application.h>

namespace cpp_warships::head {
    ChangeThemeIntent::ChangeThemeIntent(std::string themeName)
        : themeName_(std::move(themeName)) {}

    void ChangeThemeIntent::applyTo(const IntentContext& context) const {
        context.session.changeTheme(themeName_);
    }
} // namespace cpp_warships::head
