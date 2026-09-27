#include <build_views.h>

#include <memory>

#include <game_tui/views/BattleView.h>
#include <game_tui/views/MenuView.h>
#include <game_tui/views/PlacementView.h>
#include <game_tui/views/plain/PlainBattleView.h>
#include <game_tui/views/plain/PlainMenuView.h>
#include <game_tui/views/plain/PlainPlacementView.h>

namespace cpp_warships::application {
    namespace {
        /** @brief The views drawn for an interactive terminal: colour, borders and a mouse. */
        [[nodiscard]] game_tui::ViewFactory interactiveViews() {
            return game_tui::ViewFactory{
                    .menu =
                            [](const game_tui::Theme& theme,
                               const game_tui::MenuState& state,
                               game_tui::MatchInProgressQuery hasMatch) {
                                return std::make_unique<game_tui::MenuView>(
                                        theme,
                                        state,
                                        std::move(hasMatch)
                                );
                            },
                    .placement =
                            [](const game_tui::Theme& theme,
                               game_tui::MatchQuery match,
                               const game_tui::PlacementState& state) {
                                return std::make_unique<game_tui::PlacementView>(
                                        theme,
                                        std::move(match),
                                        state
                                );
                            },
                    .battle =
                            [](const game_tui::Theme& theme,
                               game_tui::MatchQuery match,
                               const game_tui::BattleJournal& journal,
                               const game_tui::BattleState& state) {
                                return std::make_unique<game_tui::BattleView>(
                                        theme,
                                        std::move(match),
                                        journal,
                                        state
                                );
                            }
            };
        }

        /** @brief The views printed as plain text, the way the console game used to read. */
        [[nodiscard]] game_tui::ViewFactory plainViews() {
            return game_tui::ViewFactory{
                    .menu =
                            [](const game_tui::Theme& theme,
                               const game_tui::MenuState& state,
                               game_tui::MatchInProgressQuery hasMatch) {
                                return std::make_unique<game_tui::PlainMenuView>(
                                        theme,
                                        state,
                                        std::move(hasMatch)
                                );
                            },
                    .placement =
                            [](const game_tui::Theme& theme,
                               game_tui::MatchQuery match,
                               const game_tui::PlacementState& state) {
                                return std::make_unique<game_tui::PlainPlacementView>(
                                        theme,
                                        std::move(match),
                                        state
                                );
                            },
                    .battle =
                            [](const game_tui::Theme& theme,
                               game_tui::MatchQuery match,
                               const game_tui::BattleJournal& journal,
                               const game_tui::BattleState& state) {
                                return std::make_unique<game_tui::PlainBattleView>(
                                        theme,
                                        std::move(match),
                                        journal,
                                        state
                                );
                            }
            };
        }
    } // namespace

    game_tui::ViewFactory buildViewFactory(const ShellKind kind) {
        return kind == ShellKind::PlainTerminal ? plainViews() : interactiveViews();
    }
} // namespace cpp_warships::application
