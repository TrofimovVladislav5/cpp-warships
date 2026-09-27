#pragma once

#include <optional>

#include <application/flow/Match.h>
#include <application/flow/RandomEngine.h>
#include <application/model/BattleJournal.h>

namespace cpp_warships::model {
    /** @brief The match being played and the story of it: the one thing every behaviour
     *  works on. Holding them together is what keeps the log and the board from drifting. */
    class MatchInPlay {
    public:
        explicit MatchInPlay(flow::RandomEngine& randomEngine) noexcept;

        [[nodiscard]] bool hasMatch() const noexcept;

        /** @brief The match in play. Throws NoMatchInPlayException when there is none,
         *  so a caller that forgot to ask hasMatch first is told rather than left guessing. */
        [[nodiscard]] const flow::Match& match() const;

        /** @brief The match in play, to act on. Only a behaviour reaches for this. */
        [[nodiscard]] flow::Match& editableMatch();

        [[nodiscard]] const BattleJournal& journal() const noexcept;
        [[nodiscard]] flow::RandomEngine& randomEngine() const noexcept;

        /** @brief Puts @p match in play in place of whatever was, and starts a fresh log. */
        void replaceWith(flow::Match match);

        /** @brief Moves whatever the match has to say into the log. */
        void recordEvents();

    private:
        flow::RandomEngine& randomEngine_;
        std::optional<flow::Match> match_;
        BattleJournal journal_;
    };
} // namespace cpp_warships::model
