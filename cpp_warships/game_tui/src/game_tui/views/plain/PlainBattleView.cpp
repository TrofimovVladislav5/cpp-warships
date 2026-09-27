#include <game_tui/views/plain/PlainBattleView.h>

#include <algorithm>
#include <cstddef>
#include <deque>
#include <string>
#include <utility>
#include <vector>

#include <game_tui/views/CoordinateLabel.h>
#include <game_tui/views/EventNarration.h>
#include <game_tui/views/plain/PlainBoardGrid.h>
#include <game_tui/views/plain/PlainFrame.h>

namespace cpp_warships::game_tui {
    namespace {
        [[nodiscard]] std::string skillBankLine(const game_flow::Match& match) {
            const std::deque<game_flow::SkillKind>& banked = match.skills().pending();
            if (banked.empty()) {
                return "SKILLS  none in the bank";
            }

            std::string line = "SKILLS  ";
            for (std::size_t position = 0; position < banked.size(); ++position) {
                line += position > 0 ? ", " : "";
                line += skillName(banked[position]);
            }

            return line;
        }

        /** @brief The window on the story, newest first and scrolled back the same way the
         *  interactive log scrolls, so the paging keys mean the same thing here. */
        [[nodiscard]] std::vector<std::string> journalLines(
                const BattleJournal& journal,
                const Theme& theme,
                const int skipped
        ) {
            const std::deque<game_flow::MatchEvent>& entries = journal.entries();
            const auto total = static_cast<int>(entries.size());
            const int from = std::clamp(skipped, 0, furthestLogScroll(total));

            std::vector<std::string> lines{
                    from > 0 ? "LOG  (" + std::to_string(from) + " back)" : "LOG"
            };

            for (int step = 0; step < LOG_VISIBLE_LINES; ++step) {
                const int index = total - 1 - from - step;
                if (index < 0) {
                    break;
                }

                lines.push_back(
                        "  " + narrate(entries[static_cast<std::size_t>(index)], theme).text
                );
            }

            return lines;
        }
    } // namespace

    PlainBattleView::PlainBattleView(
            const Theme& theme,
            MatchQuery match,
            const BattleJournal& journal,
            const BattleState& state
    )
        : theme_(theme)
        , match_(std::move(match))
        , journal_(journal)
        , state_(state) {}

    InputEvent PlainBattleView::interpret(const Keystroke& stroke) const {
        return {.stroke = stroke};
    }

    Frame PlainBattleView::render(int, int) {
        const game_flow::Match& match = match_();
        const bool isFinished = match.phase() == game_flow::MatchPhase::Finished;
        const std::string turn = isFinished             ? "YOUR FLEET IS GONE"
                                 : match.isPlayerTurn() ? "your turn"
                                                        : "the enemy's turn";

        std::vector<std::string> lines{
                "BATTLE   round " + std::to_string(match.roundNumber()) + "   " + turn,
                "",
                "YOUR WATERS"
        };

        const std::vector<std::string> ownWaters =
                plainBoardLines(match.playerBoard(), game_core::Visibility::Owner, {});
        lines.insert(lines.end(), ownWaters.begin(), ownWaters.end());

        lines.emplace_back("");
        lines.emplace_back("ENEMY WATERS");

        const PlainBoardOverlay aim{.cursor = state_.target};
        const std::vector<std::string> enemyWaters =
                plainBoardLines(match.computerBoard(), game_core::Visibility::Opponent, aim);
        lines.insert(lines.end(), enemyWaters.begin(), enemyWaters.end());

        lines.emplace_back("");
        lines.push_back(plainBoardLegend());
        lines.emplace_back("");
        lines.push_back("  aiming at " + coordinateLabel(state_.target));
        lines.emplace_back("");
        lines.push_back(skillBankLine(match));
        lines.emplace_back("");

        const std::vector<std::string> log = journalLines(journal_, theme_, state_.logScroll);
        lines.insert(lines.end(), log.begin(), log.end());

        lines.emplace_back("");

        if (!isFinished) {
            lines.push_back(plainKeyLine("arrows", "take aim"));
            lines.push_back(plainKeyLine("enter", "fire"));
            lines.push_back(plainKeyLine("k", "use the next skill"));
            lines.push_back(plainKeyLine("pgup", "further back through the log"));
            lines.push_back(plainKeyLine("f2", "save the match"));
        }

        lines.push_back(plainKeyLine("esc", "back to the menu"));
        lines.emplace_back("");

        return frameOfLines(lines);
    }
} // namespace cpp_warships::game_tui
