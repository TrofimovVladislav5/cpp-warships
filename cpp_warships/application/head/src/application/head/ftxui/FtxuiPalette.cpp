#include <application/head/ftxui/FtxuiPalette.h>

#include <map>
#include <string>

namespace cpp_warships::head {
    namespace {
        /** @brief Every colour we have ever asked FTXUI for, under the code it
         * prints.
         */
        std::map<std::string, Color>& knownColors() {
            static std::map<std::string, Color> known;
            return known;
        }
    }  // namespace

    ftxui::Color toFtxuiColor(const Color value) {
        const ftxui::Color painted = ftxui::Color::RGB(value.red, value.green, value.blue);
        knownColors()[painted.Print(false)] = value;
        return painted;
    }

    std::optional<Color> colorOf(const ftxui::Color& painted) {
        const std::map<std::string, Color>& known = knownColors();
        const auto remembered = known.find(painted.Print(false));

        return remembered == known.end() ? std::nullopt : std::optional{remembered->second};
    }

    ftxui::Decorator color(const Color value) {
        return ftxui::color(toFtxuiColor(value));
    }

    ftxui::Decorator bgcolor(const Color value) {
        return ftxui::bgcolor(toFtxuiColor(value));
    }
}  // namespace cpp_warships::head
