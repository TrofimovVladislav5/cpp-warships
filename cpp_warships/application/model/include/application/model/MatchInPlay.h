#pragma once

#include <application/flow/Match.h>
#include <application/flow/RandomEngine.h>
#include <application/model/BattleJournal.h>

#include <optional>
#include <string>

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

        /** @brief Puts @p match in play carrying @p story, and remembers it came from the
         * save called @p fromSlot so that saving again updates that one. */
        void replaceWith(flow::Match match, const flow::MatchEventLog& story, std::string fromSlot);

        /** @brief The save this match was loaded from, when it was loaded from one. */
        [[nodiscard]] const std::optional<std::string>& loadedFrom() const noexcept;

        /** @brief Says the match now lives in the save called @p slot. */
        void rememberSlot(std::string slot);

        /** @brief Moves whatever the match has to say into the log. */
        void recordEvents();

    private:
        flow::RandomEngine& randomEngine_;
        std::optional<flow::Match> match_;
        BattleJournal journal_;
        std::optional<std::string> loadedFrom_;
    };
}  // namespace cpp_warships::model
