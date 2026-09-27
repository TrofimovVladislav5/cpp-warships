#include <build_views.h>

#include <memory>

#include <application/head/views/BattleView.h>
#include <application/head/views/MenuView.h>
#include <application/head/views/PlacementView.h>
#include <application/head/views/plain/PlainBattleView.h>
#include <application/head/views/plain/PlainMenuView.h>
#include <application/head/views/plain/PlainPlacementView.h>

namespace cpp_warships::application {
    namespace {
        /** @brief The views drawn for an interactive terminal: colour, borders and a mouse. */
        [[nodiscard]] head::ViewFactory interactiveViews() {
            return head::ViewFactory{
                    .menu =
                            [](const head::Theme& theme,
                               const head::MenuState& state,
                               head::MatchInProgressQuery hasMatch,
                               head::SavedMatchQuery hasSavedMatch) {
                                return std::make_unique<head::MenuView>(
                                        theme,
                                        state,
                                        std::move(hasMatch),
                                        std::move(hasSavedMatch)
                                );
                            },
                    .placement =
                            [](const head::Theme& theme,
                               head::MatchQuery match,
                               const head::PlacementState& state) {
                                return std::make_unique<head::PlacementView>(
                                        theme,
                                        std::move(match),
                                        state
                                );
                            },
                    .battle =
                            [](const head::Theme& theme,
                               head::MatchQuery match,
                               const model::BattleJournal& journal,
                               const head::BattleState& state) {
                                return std::make_unique<head::BattleView>(
                                        theme,
                                        std::move(match),
                                        journal,
                                        state
                                );
                            }
            };
        }

        /** @brief The views printed as plain text, the way the console game used to read. */
        [[nodiscard]] head::ViewFactory plainViews() {
            return head::ViewFactory{
                    .menu =
                            [](const head::Theme& theme,
                               const head::MenuState& state,
                               head::MatchInProgressQuery hasMatch,
                               head::SavedMatchQuery hasSavedMatch) {
                                return std::make_unique<head::PlainMenuView>(
                                        theme,
                                        state,
                                        std::move(hasMatch),
                                        std::move(hasSavedMatch)
                                );
                            },
                    .placement =
                            [](const head::Theme& theme,
                               head::MatchQuery match,
                               const head::PlacementState& state) {
                                return std::make_unique<head::PlainPlacementView>(
                                        theme,
                                        std::move(match),
                                        state
                                );
                            },
                    .battle =
                            [](const head::Theme& theme,
                               head::MatchQuery match,
                               const model::BattleJournal& journal,
                               const head::BattleState& state) {
                                return std::make_unique<head::PlainBattleView>(
                                        theme,
                                        std::move(match),
                                        journal,
                                        state
                                );
                            }
            };
        }
    } // namespace

    head::ViewFactory buildViewFactory(const ShellKind kind) {
        return kind == ShellKind::PlainTerminal ? plainViews() : interactiveViews();
    }
} // namespace cpp_warships::application
