#pragma once

#include <application/head/common/Theme.h>

#include <string>

namespace cpp_warships::head {
    /** @brief Which palette everything is dressed in. This is presentation
     * state through and through: the rules neither set it nor care, so the
     * model never sees it. */
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
}  // namespace cpp_warships::head
