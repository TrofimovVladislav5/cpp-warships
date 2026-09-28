#pragma once

#include <application/flow/RandomEngine.h>
#include <application/model/MatchInPlay.h>
#include <application/model/behaviors/MatchBehavior.h>
#include <application/model/behaviors/SaveBehavior.h>

namespace cpp_warships::flow {
    class Match;
}

namespace cpp_warships::model {
    class BattleJournal;
}

namespace cpp_warships::persistence {
    class SaveArchive;
}

namespace cpp_warships::model {
    /** @brief The game being played: what it currently is, and everything that can be done to
     * it. */
    class WarshipsGame {
    public:
        /** @brief A game played out with @p randomEngine and saved into @p
         * saveArchive, both of which must outlive it. */
        WarshipsGame(
            flow::RandomEngine& randomEngine,
            persistence::SaveArchive& saveArchive
        ) noexcept;

        /** @brief The behaviours hold references to state inside this object, so a copy would
         * leave them pointing at the original. */
        WarshipsGame(const WarshipsGame&) = delete;
        WarshipsGame& operator=(const WarshipsGame&) = delete;
        WarshipsGame(WarshipsGame&&) = delete;
        WarshipsGame& operator=(WarshipsGame&&) = delete;
        ~WarshipsGame() = default;

        [[nodiscard]] bool hasMatch() const noexcept;
        [[nodiscard]] const flow::Match& match() const;
        [[nodiscard]] const BattleJournal& journal() const noexcept;

        /** @brief Starting a match, laying out a fleet, and fighting it. */
        [[nodiscard]] behaviors::MatchBehavior& play() noexcept;

        /** @brief Putting a match away and picking it back up. */
        [[nodiscard]] behaviors::SaveBehavior& saves() noexcept;
        [[nodiscard]] const behaviors::SaveBehavior& saves() const noexcept;

    private:
        MatchInPlay inPlay_;
        behaviors::MatchBehavior play_;
        behaviors::SaveBehavior saves_;
    };
}  // namespace cpp_warships::model
