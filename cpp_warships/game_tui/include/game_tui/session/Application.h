#pragma once

#include <optional>
#include <string>

#include <game_core/Coordinate.h>
#include <game_core/Direction.h>
#include <game_flow/Match.h>
#include <game_flow/RandomEngine.h>
#include <game_tui/Theme.h>
#include <game_tui/session/BattleJournal.h>

namespace cpp_warships::game_tui {
    /** @brief Everything the current session is: the match in play, what has happened in it,
     *  and the settings it is played under. It knows nothing of screens or of the terminal. */
    class Application {
    public:
        explicit Application(game_flow::RandomEngine& randomEngine);

        [[nodiscard]] const Theme& theme() const noexcept;
        [[nodiscard]] bool hasMatch() const noexcept;
        [[nodiscard]] const game_flow::Match& match() const;
        [[nodiscard]] const BattleJournal& journal() const noexcept;

        void startNewMatch(int boardSize);
        void changeTheme(const std::string& themeName);

        void placeShip(game_core::Coordinate origin, game_core::Direction direction, int length);
        void removeShipAt(game_core::Coordinate coordinate);
        void shuffleFleet();

        /** @brief Opens fire, once the fleet is laid out. @return whether the battle began. */
        bool beginBattle();

        void fireAt(game_core::Coordinate coordinate);
        void useSkill(std::optional<game_core::Coordinate> target);

    private:
        /** @brief Lets the match hand play on, then keeps what happened. */
        void settleTurn();

        game_flow::RandomEngine& randomEngine_;
        Theme theme_;
        std::optional<game_flow::Match> match_;
        BattleJournal journal_;
    };
} // namespace cpp_warships::game_tui
