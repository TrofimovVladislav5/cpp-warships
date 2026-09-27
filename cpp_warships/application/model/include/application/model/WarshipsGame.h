#pragma once

#include <application/flow/Match.h>
#include <application/flow/RandomEngine.h>
#include <application/model/BattleJournal.h>
#include <application/model/MatchInPlay.h>
#include <application/model/behaviors/MatchBehavior.h>
#include <application/model/behaviors/SaveBehavior.h>
#include <application/persistence/SaveArchive.h>

namespace cpp_warships::model {
    /** @brief The game being played: what it currently is, and everything that can be done
     *  to it. Reading is offered here directly; changing it goes through a behaviour. */
    class WarshipsGame {
    public:
        /** @brief A game played out with @p randomEngine and saved into @p saveArchive,
         *  both of which must outlive it. */
        WarshipsGame(
                flow::RandomEngine& randomEngine,
                persistence::SaveArchive& saveArchive
        ) noexcept;

        /** @brief The behaviours hold references to state inside this object, so a copy
         *  would leave them pointing at the original. There is one game, in one place. */
        WarshipsGame(const WarshipsGame&) = delete;
        WarshipsGame& operator=(const WarshipsGame&) = delete;
        WarshipsGame(WarshipsGame&&) = delete;
        WarshipsGame& operator=(WarshipsGame&&) = delete;
        ~WarshipsGame() = default;

        [[nodiscard]] bool hasMatch() const noexcept;
        [[nodiscard]] const flow::Match& match() const;
        [[nodiscard]] const BattleJournal& journal() const noexcept;

        /** @brief Starting a match, laying out a fleet, and fighting it. */
        [[nodiscard]] MatchBehavior& play() noexcept;

        /** @brief Putting a match away and picking it back up. */
        [[nodiscard]] SaveBehavior& saves() noexcept;
        [[nodiscard]] const SaveBehavior& saves() const noexcept;

    private:
        MatchInPlay inPlay_;
        MatchBehavior play_;
        SaveBehavior saves_;
    };
} // namespace cpp_warships::model
