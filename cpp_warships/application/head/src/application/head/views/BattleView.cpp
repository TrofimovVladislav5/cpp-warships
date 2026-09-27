#include <application/head/views/BattleView.h>

#include <application/head/views/ftxui_bridge/FtxuiPalette.h>

#include <algorithm>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <deque>
#include <optional>

#include <application/head/views/EventNarration.h>
#include <application/head/views/KeyHint.h>
#include <application/head/views/ftxui_bridge/FtxuiNotices.h>

namespace cpp_warships::head {
    namespace {
        constexpr int SIDE_PANEL_WIDTH = 46;
        constexpr int SKILL_ORDER_LINES = 4;

        ftxui::Element standing(const Theme& theme, const flow::Match& match) {
            if (match.phase() == flow::MatchPhase::Finished) {
                return ftxui::text("YOUR FLEET IS GONE") | ftxui::bold | color(theme.danger);
            }

            if (match.isPlayerTurn()) {
                return ftxui::text("your turn") | ftxui::bold | color(theme.success);
            }

            return ftxui::text("the enemy fires") | ftxui::bold | color(theme.danger);
        }

        ftxui::Element header(const Theme& theme, const flow::Match& match) {
            std::vector<ftxui::Element> parts{
                    ftxui::text("BATTLE") | ftxui::bold | color(theme.accent),
                    ftxui::filler(),
                    ftxui::text("round " + std::to_string(match.roundNumber())) |
                            color(theme.textMuted),
                    ftxui::text("   "),
                    standing(theme, match)
            };

            if (match.isDoubleDamageArmed()) {
                parts.push_back(ftxui::text("   "));
                parts.push_back(ftxui::text("double damage armed") | color(theme.accent));
            }

            return ftxui::hbox(std::move(parts));
        }

        ftxui::Element titledBoard(
                const Theme& theme,
                const std::string& title,
                ftxui::Element board
        ) {
            return ftxui::vbox(
                    {ftxui::text(title) | ftxui::bold | color(theme.textMuted),
                     ftxui::separator() | color(theme.border),
                     std::move(board) | ftxui::center | ftxui::flex}
            );
        }

        /** @brief How many of each skill are banked, in a settled order rather than queue order. */
        std::vector<std::pair<flow::SkillKind, int>> bankedByKind(
                const flow::Match& match
        ) {
            const std::deque<flow::SkillKind>& banked = match.skills().pending();

            std::vector<std::pair<flow::SkillKind, int>> tally;
            for (const flow::SkillKind kind : flow::ALL_SKILL_KINDS) {
                const auto held = static_cast<int>(std::count(banked.begin(), banked.end(), kind));
                if (held > 0) {
                    tally.emplace_back(kind, held);
                }
            }

            return tally;
        }

        /** @brief What the next skill will do when spent, said where the queue can show it. */
        ftxui::Element nextMarker(const Theme& theme, const flow::Match& match) {
            const std::string label = match.nextSkillNeedsTarget() ? "next, where you aim" : "next";
            return ftxui::text(label) | color(theme.accent);
        }

        /** @brief One row saying how many of each kind are held, whatever order they sit in. */
        ftxui::Element skillCounts(const Theme& theme, const flow::Match& match) {
            if (match.skills().isEmpty()) {
                return ftxui::text("none banked") | color(theme.textMuted);
            }

            std::vector<ftxui::Element> chips;
            for (const auto& [kind, held] : bankedByKind(match)) {
                if (!chips.empty()) {
                    chips.push_back(ftxui::text("  "));
                }

                chips.push_back(ftxui::text(skillName(kind)) | color(theme.textMuted));
                chips.push_back(
                        ftxui::text(" " + std::to_string(held)) | ftxui::bold | color(theme.accent)
                );
            }

            return ftxui::hbox(std::move(chips));
        }

        /** @brief The bank in the order it will be spent, oldest first, the next one marked.
         *  Always the same height: a long bank gives up its tail to a line counting the rest. */
        ftxui::Element skillOrder(const Theme& theme, const flow::Match& match) {
            const std::deque<flow::SkillKind>& banked = match.skills().pending();
            const auto fits = static_cast<std::size_t>(SKILL_ORDER_LINES);
            const std::size_t listed = banked.size() > fits ? fits - 1 : banked.size();

            std::vector<ftxui::Element> rows;
            for (std::size_t position = 0; position < listed; ++position) {
                const bool isNext = position == 0;
                rows.push_back(
                        ftxui::hbox(
                                {ftxui::text(std::to_string(position + 1) + "  ") |
                                         color(theme.textMuted),
                                 ftxui::text(skillName(banked[position])) |
                                         color(isNext ? theme.text : theme.textMuted),
                                 ftxui::filler(),
                                 isNext ? nextMarker(theme, match) : ftxui::text("")}
                        )
                );
            }

            if (banked.size() > listed) {
                const std::string rest = std::to_string(banked.size() - listed);
                rows.push_back(ftxui::text("+" + rest + " more skills") | color(theme.textMuted));
            }

            while (rows.size() < fits) {
                rows.push_back(ftxui::text(""));
            }

            return ftxui::vbox(std::move(rows)) |
                   ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, SKILL_ORDER_LINES);
        }

        ftxui::Element skillBank(const Theme& theme, const flow::Match& match) {
            return ftxui::vbox(
                    {ftxui::text("SKILLS") | ftxui::bold | color(theme.accent),
                     skillCounts(theme, match),
                     skillOrder(theme, match)}
            );
        }

        /** @brief A fixed window on the story so far, newest first, scrolled back by @p skipped.
         *  It keeps its height whether the log is empty or a hundred lines long. */
        ftxui::Element journalLines(const Theme& theme, const model::BattleJournal& journal, int skipped) {
            const std::deque<flow::MatchEvent>& entries = journal.entries();
            const int total = static_cast<int>(entries.size());
            const int from = std::clamp(skipped, 0, furthestLogScroll(total));

            std::vector<ftxui::Element> lines;
            for (int step = 0; step < LOG_VISIBLE_LINES; ++step) {
                const int index = total - 1 - from - step;
                if (index < 0) {
                    lines.push_back(ftxui::text(""));
                    continue;
                }

                const EventLine line = narrate(entries[static_cast<std::size_t>(index)], theme);
                lines.push_back(ftxui::text(line.text) | color(line.color));
            }

            return ftxui::vbox(std::move(lines)) |
                   ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, LOG_VISIBLE_LINES);
        }

        ftxui::Element journalHeading(
                const Theme& theme,
                const model::BattleJournal& journal,
                int skipped
        ) {
            const int total = static_cast<int>(journal.entries().size());
            const int from = std::clamp(skipped, 0, furthestLogScroll(total));

            std::vector<ftxui::Element> parts{
                    ftxui::text("LOG") | ftxui::bold | color(theme.accent),
                    ftxui::filler()
            };

            if (from > 0) {
                parts.push_back(
                        ftxui::text(std::to_string(from) + " back") | color(theme.textMuted)
                );
            }

            return ftxui::hbox(std::move(parts));
        }

        ftxui::Element legend(const Theme& theme, const flow::Match& match) {
            if (match.phase() == flow::MatchPhase::Finished) {
                return keyLegend({keyHint(theme, "esc", "back to the menu")});
            }

            std::vector<ftxui::Element> hints{
                    keyHint(theme, "arrows", "take aim"),
                    keyHint(theme, "enter", "fire"),
                    keyHint(theme, "f2", "save the match"),
                    keyHint(theme, "esc", "back to the menu")
            };

            if (!match.skills().isEmpty()) {
                hints.insert(hints.begin() + 2, keyHint(theme, "k", "use the next skill"));
            }

            return keyLegend(std::move(hints));
        }
    } // namespace

    BattleView::BattleView(
            const PresentationContext& context,
            GridGeometry& geometry
    ) noexcept
        : context_(context)
        , ownWatersView_(geometry, ScreenRegion::OwnWaters)
        , enemyWatersView_(geometry, ScreenRegion::EnemyWaters)
        , geometry_(geometry) {}

    void BattleView::publishLogGeometry() const {
        if (logBox_.x_max < logBox_.x_min) {
            return;
        }

        geometry_.rememberLog(
                logBox_.x_min,
                logBox_.y_min,
                logBox_.x_max - logBox_.x_min + 1,
                logBox_.y_max - logBox_.y_min + 1
        );
    }

    ftxui::Element BattleView::renderElement() {
        publishLogGeometry();

        const Theme& theme = context_.theme();
        const flow::Match& match = context_.game().match();
        const model::BattleJournal& journal = context_.game().journal();
        const BattleState& state = context_.state().battle;

        BoardOverlay ownOverlay;
        BoardOverlay enemyOverlay;
        enemyOverlay.cursor = state.target;

        ftxui::Element ownWaters = ownWatersView_.render(
                match.playerBoard(),
                core::Visibility::Owner,
                theme,
                ownOverlay
        );
        ftxui::Element enemyWaters = enemyWatersView_.render(
                match.computerBoard(),
                core::Visibility::Opponent,
                theme,
                enemyOverlay
        );

        return ftxui::vbox(
                       {header(theme, match),
                        ftxui::separator() | color(theme.border),
                        ftxui::hbox(
                                {titledBoard(theme, "YOUR WATERS", std::move(ownWaters)) |
                                         ftxui::flex,
                                 ftxui::separator() | color(theme.border),
                                 titledBoard(theme, "ENEMY WATERS", std::move(enemyWaters)) |
                                         ftxui::flex,
                                 ftxui::separator() | color(theme.border),
                                 ftxui::vbox(
                                         {skillBank(theme, match),
                                          ftxui::separator() | color(theme.border),
                                          legend(theme, match),
                                          ftxui::separator() | color(theme.border),
                                          journalHeading(theme, journal, state.logScroll),
                                          journalLines(theme, journal, state.logScroll) |
                                                  ftxui::reflect(logBox_),
                                          ftxui::filler()}
                                 ) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, SIDE_PANEL_WIDTH)}
                        ) | ftxui::flex,
                        noticeBlock(theme, context_.application())}
               ) |
               ftxui::border | color(theme.border) | bgcolor(theme.background) | ftxui::flex;
    }
} // namespace cpp_warships::head
