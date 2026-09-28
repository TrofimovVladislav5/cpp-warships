#pragma once

#include <application/head/common/Theme.h>

#include <string>

namespace cpp_warships::head::common {
    /** @brief Which palette everything is dressed in. */
    class ThemeSelection {
    public:
        ThemeSelection();

        [[nodiscard]] const Theme& current() const noexcept;

        /** @brief Dresses everything in the theme called @p themeName, if there
         * is one. */
        void change(const std::string& themeName);

    private:
        Theme theme_;
    };
}  // namespace cpp_warships::head::common
