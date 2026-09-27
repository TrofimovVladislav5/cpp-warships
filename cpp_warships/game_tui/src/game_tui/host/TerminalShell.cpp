#include <game_tui/host/TerminalShell.h>

#include <algorithm>
#include <cctype>
#include <istream>
#include <map>
#include <ostream>
#include <string>

#include <game_tui/screens/ScreenNavigator.h>
#include <game_tui/views/GameView.h>

namespace cpp_warships::game_tui {
    namespace {
        const std::string PROMPT = "> ";

        /** @brief The typed word stripped of surrounding blanks and folded to lower case,
         *  so that "  Up " and "up" ask for the same thing. */
        [[nodiscard]] std::string normalised(const std::string& command) {
            const auto isBlank = [](const unsigned char letter) {
                return std::isspace(letter) != 0;
            };
            const auto toLower = [](const unsigned char letter) {
                return static_cast<char>(std::tolower(letter));
            };

            const auto first = std::find_if_not(command.begin(), command.end(), isBlank);
            const auto last = std::find_if_not(command.rbegin(), command.rend(), isBlank).base();

            if (first >= last) {
                return {};
            }
            std::string trimmed(first, last);
            std::transform(trimmed.begin(), trimmed.end(), trimmed.begin(), toLower);
            return trimmed;
        }

        /** @brief The keystroke @p command stands for: a name for the keys that cannot be
         *  typed as text, and otherwise the first letter typed. */
        [[nodiscard]] Keystroke keystrokeFromCommand(const std::string& command) {
            static const std::map<std::string, Key> NAMED_KEYS = {
                    {"", Key::Enter},
                    {"enter", Key::Enter},
                    {"up", Key::ArrowUp},
                    {"down", Key::ArrowDown},
                    {"left", Key::ArrowLeft},
                    {"right", Key::ArrowRight},
                    {"esc", Key::Escape},
                    {"tab", Key::Tab},
                    {"back", Key::Backspace},
                    {"pgup", Key::PageUp},
                    {"pgdn", Key::PageDown},
                    {"f2", Key::SaveKey},
                    {"f3", Key::LoadKey},
            };

            const auto namedKey = NAMED_KEYS.find(command);
            if (namedKey != NAMED_KEYS.end()) {
                return Keystroke{.key = namedKey->second};
            }

            return Keystroke{.key = Key::Character, .character = command.substr(0, 1)};
        }
    } // namespace

    TerminalShell::TerminalShell(std::istream& input, std::ostream& output)
        : input_(input)
        , output_(output) {}

    void TerminalShell::run(ScreenNavigator& navigator) {
        isRunning_ = true;

        while (isRunning_) {
            drawFrame(navigator.activeScreen().view());

            const std::optional<Keystroke> stroke = readKeystroke();
            if (!stroke.has_value()) {
                return;
            }

            navigator.activeScreen().handleEvent(*stroke);
        }
    }

    void TerminalShell::requestQuit() {
        isRunning_ = false;
    }

    void TerminalShell::drawFrame(GameView& view) const {
        output_ << frameToText(view.render(0, 0)) << PROMPT;
        output_.flush();
    }

    std::optional<Keystroke> TerminalShell::readKeystroke() const {
        std::string command;
        if (!std::getline(input_, command)) {
            return std::nullopt;
        }

        return keystrokeFromCommand(normalised(command));
    }
} // namespace cpp_warships::game_tui
