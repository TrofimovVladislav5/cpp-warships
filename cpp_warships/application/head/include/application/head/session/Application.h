#pragma once

#include <optional>
#include <string>

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/flow/Match.h>
#include <application/flow/RandomEngine.h>
#include <application/persistence/SaveArchive.h>
#include <application/head/Theme.h>
#include <application/head/session/BattleJournal.h>

namespace cpp_warships::head {
    /** @brief Everything the current session is: the match in play, what has happened in it,
     *  and the settings it is played under. It knows nothing of screens or of the terminal. */
    class Application {
    public:
        /** @brief A session played with @p randomEngine, saving into @p saveArchive,
         *  which must outlive it. */
        Application(
                flow::RandomEngine& randomEngine,
                persistence::SaveArchive& saveArchive
        );

        [[nodiscard]] const Theme& theme() const noexcept;
        [[nodiscard]] bool hasMatch() const noexcept;
        [[nodiscard]] const flow::Match& match() const;
        [[nodiscard]] const BattleJournal& journal() const noexcept;

        /** @brief Whether there is a saved match waiting to be picked back up. */
        [[nodiscard]] bool hasSavedMatch() const;

        void startNewMatch(int boardSize);
        void changeTheme(const std::string& themeName);

        void placeShip(core::Coordinate origin, core::Direction direction, int length);
        void removeShipAt(core::Coordinate coordinate);
        void shuffleFleet();

        /** @brief Opens fire, once the fleet is laid out. @return whether the battle began. */
        bool beginBattle();

        void fireAt(core::Coordinate coordinate);
        void useSkill(std::optional<core::Coordinate> target);

        /** @brief Writes the match in play over the save.
         *  @return whether there was a match, and it was stored. */
        bool saveMatch();

        /** @brief Replaces whatever is in play with the saved match.
         *  @return whether a save was there and could be read. */
        bool loadMatch();

    private:
        /** @brief Lets the match hand play on, then keeps what happened. */
        void settleTurn();

        flow::RandomEngine& randomEngine_;
        persistence::SaveArchive& saveArchive_;
        Theme theme_;
        std::optional<flow::Match> match_;
        BattleJournal journal_;
    };
} // namespace cpp_warships::head
