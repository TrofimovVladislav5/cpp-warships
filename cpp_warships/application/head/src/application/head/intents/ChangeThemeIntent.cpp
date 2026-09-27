#include <application/head/intents/ChangeThemeIntent.h>

#include <string>
#include <utility>

#include <application/head/ThemeSelection.h>
#include <application/head/intents/IntentContext.h>

namespace cpp_warships::head {
    ChangeThemeIntent::ChangeThemeIntent(std::string themeName)
        : themeName_(std::move(themeName)) {}

    void ChangeThemeIntent::applyTo(const IntentContext& context) const {
        context.theme.change(themeName_);
    }
} // namespace cpp_warships::head
