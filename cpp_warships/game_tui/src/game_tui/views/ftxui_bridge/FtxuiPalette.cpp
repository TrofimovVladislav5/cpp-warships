#include <game_tui/views/ftxui_bridge/FtxuiPalette.h>

#include <map>
#include <string>

namespace cpp_warships::game_tui {
    namespace {
        /** @brief Every colour we have ever asked FTXUI for, under the code it prints.
         *  FTXUI settles a colour when it is built, dropping a true colour to the nearest
         *  palette entry on a terminal that cannot do better, and it keeps its channels
         *  private. So the only honest way back is to remember what we asked for. Two
         *  colours that settle on the same entry are the same colour to that terminal,
         *  which is why the later one may stand for both. Written from drawing only,
         *  which the game does on one thread. */
        std::map<std::string, Color>& knownColors() {
            static std::map<std::string, Color> known;
            return known;
        }
    } // namespace

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
} // namespace cpp_warships::game_tui
