#pragma once

#include <application/flow/Match.h>
#include <application/flow/RandomEngine.h>
#include <application/model/BattleJournal.h>

#include <optional>

namespace cpp_warships::model {
    /** @brief The match being played and the story of it: the one thing every behaviour works
     * on. */
    class MatchInPlay {
    public:
        explicit MatchInPlay(flow::RandomEngine& randomEngine) noexcept;

        [[nodiscard]] bool hasMatch() const noexcept;

        /** @brief The match in play. */
        [[nodiscard]] const flow::Match& match() const;

        /** @brief The match in play, to act on. */
        [[nodiscard]] flow::Match& editableMatch();

        [[nodiscard]] const BattleJournal& journal() const noexcept;
        [[nodiscard]] flow::RandomEngine& randomEngine() const noexcept;

        /** @brief Puts @p match in play in place of whatever was, and starts a
         * fresh log. */
        void replaceWith(flow::Match match);

        /** @brief Puts @p match in play carrying @p story, which is how a loaded game comes back
         * with the log it was saved with rather than starting silent. */
        void replaceWith(flow::Match match, const flow::MatchEventLog& story);

        /** @brief Moves whatever the match has to say into the log. */
        void recordEvents();

    private:
        flow::RandomEngine& randomEngine_;
        std::optional<flow::Match> match_;
        BattleJournal journal_;
    };
}  // namespace cpp_warships::model
