#pragma once

#include <optional>
#include <string>

#include <game_core/Coordinate.h>
#include <game_core/Direction.h>
#include <game_flow/Match.h>
#include <game_flow/RandomEngine.h>
#include <game_persistence/SaveArchive.h>
#include <game_tui/Theme.h>
#include <game_tui/session/BattleJournal.h>

namespace cpp_warships::game_tui {
    /** @brief Everything the current session is: the match in play, what has happened in it,
     *  and the settings it is played under. It knows nothing of screens or of the terminal. */
    class Application {
    public:
        /** @brief A session played with @p randomEngine, saving into @p saveArchive,
         *  which must outlive it. */
        Application(
                game_flow::RandomEngine& randomEngine,
                game_persistence::SaveArchive& saveArchive
        );

        [[nodiscard]] const Theme& theme() const noexcept;
        [[nodiscard]] bool hasMatch() const noexcept;
        [[nodiscard]] const game_flow::Match& match() const;
        [[nodiscard]] const BattleJournal& journal() const noexcept;

        /** @brief Whether there is a saved match waiting to be picked back up. */
        [[nodiscard]] bool hasSavedMatch() const;

        void startNewMatch(int boardSize);
        void changeTheme(const std::string& themeName);

        void placeShip(game_core::Coordinate origin, game_core::Direction direction, int length);
        void removeShipAt(game_core::Coordinate coordinate);
        void shuffleFleet();

        /** @brief Opens fire, once the fleet is laid out. @return whether the battle began. */
        bool beginBattle();

        void fireAt(game_core::Coordinate coordinate);
        void useSkill(std::optional<game_core::Coordinate> target);

        /** @brief Writes the match in play over the save.
         *  @return whether there was a match, and it was stored. */
        bool saveMatch();

        /** @brief Replaces whatever is in play with the saved match.
         *  @return whether a save was there and could be read. */
        bool loadMatch();

    private:
        /** @brief Lets the match hand play on, then keeps what happened. */
        void settleTurn();

        game_flow::RandomEngine& randomEngine_;
        game_persistence::SaveArchive& saveArchive_;
        Theme theme_;
        std::optional<game_flow::Match> match_;
        BattleJournal journal_;
    };
} // namespace cpp_warships::game_tui
