#include <game_tui/Theme.h>

#include <algorithm>

namespace cpp_warships::game_tui {
    namespace {
        Theme makeMidnightTheme() {
            return Theme{
                    .name = "midnight",
                    .background = Color{16, 18, 28},
                    .surface = Color{28, 32, 48},
                    .border = Color{64, 72, 104},
                    .text = Color{226, 232, 248},
                    .textMuted = Color{136, 146, 178},
                    .accent = Color{126, 190, 255},
                    .water = {Color{28, 40, 68}, Color{78, 94, 130}},
                    .ship = {Color{176, 188, 214}, Color{18, 22, 34}},
                    .damaged = {Color{250, 200, 90}, Color{58, 34, 4}},
                    .destroyed = {Color{206, 118, 38}, Color{40, 18, 2}},
                    .sunk = {Color{160, 34, 44}, Color{255, 228, 228}},
                    .miss = {Color{134, 134, 134}, Color{16, 16, 16}},
                    .cursor = {Color{126, 190, 255}, Color{10, 16, 30}},
                    .danger = Color{232, 92, 92},
                    .success = Color{126, 210, 150}
            };
        }

        Theme makeHarbourTheme() {
            return Theme{
                    .name = "harbour",
                    .background = Color{246, 244, 238},
                    .surface = Color{232, 228, 218},
                    .border = Color{176, 168, 152},
                    .text = Color{38, 40, 44},
                    .textMuted = Color{118, 116, 112},
                    .accent = Color{28, 104, 168},
                    .water = {Color{208, 222, 236}, Color{128, 148, 172}},
                    .ship = {Color{66, 74, 88}, Color{238, 240, 244}},
                    .damaged = {Color{244, 186, 66}, Color{48, 28, 2}},
                    .destroyed = {Color{202, 110, 24}, Color{40, 18, 2}},
                    .sunk = {Color{158, 28, 36}, Color{255, 234, 234}},
                    .miss = {Color{128, 128, 128}, Color{252, 252, 252}},
                    .cursor = {Color{28, 104, 168}, Color{240, 246, 252}},
                    .danger = Color{176, 48, 48},
                    .success = Color{46, 134, 82}
            };
        }
    } // namespace

    const std::vector<Theme>& availableThemes() {
        static const std::vector<Theme> themes{makeMidnightTheme(), makeHarbourTheme()};
        return themes;
    }

    const Theme& defaultTheme() {
        return availableThemes().front();
    }

    const Theme& themeNamed(const std::string& name) {
        const auto hasName = [&name](const Theme& theme) {
            return theme.name == name;
        };

        const auto& themes = availableThemes();
        const auto found = std::find_if(themes.begin(), themes.end(), hasName);

        return found == themes.end() ? defaultTheme() : *found;
    }
} // namespace cpp_warships::game_tui
