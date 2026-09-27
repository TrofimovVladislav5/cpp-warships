#include <application/head/ThemeSelection.h>

namespace cpp_warships::head {
    ThemeSelection::ThemeSelection()
        : theme_(defaultTheme()) {}

    const Theme& ThemeSelection::current() const noexcept {
        return theme_;
    }

    void ThemeSelection::change(const std::string& themeName) {
        theme_ = themeNamed(themeName);
    }
} // namespace cpp_warships::head
