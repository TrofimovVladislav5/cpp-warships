#pragma once

#include <functional>
#include <memory>

#include <game_tui/Queries.h>
#include <game_tui/Theme.h>
#include <game_tui/screens/BattleState.h>
#include <game_tui/screens/MenuState.h>
#include <game_tui/screens/PlacementState.h>
#include <game_tui/session/BattleJournal.h>
#include <game_tui/views/GameView.h>

namespace cpp_warships::game_tui {
    using GameViewPointer = std::unique_ptr<GameView>;

    /** @brief Makes the view for the menu, given what the menu is showing. */
    using MenuViewFactory =
            std::function<GameViewPointer(const Theme&, const MenuState&, MatchInProgressQuery)>;

    /** @brief Makes the view for the placement screen, given what it is showing. */
    using PlacementViewFactory =
            std::function<GameViewPointer(const Theme&, MatchQuery, const PlacementState&)>;

    /** @brief Makes the view for the battle screen, given what it is showing. */
    using BattleViewFactory = std::function<
            GameViewPointer(const Theme&, MatchQuery, const BattleJournal&, const BattleState&)>;

    /** @brief One whole way of presenting the game: a view for every screen there is.
     *  Swapping the presentation is swapping this, and nothing else. */
    struct ViewFactory {
        MenuViewFactory menu;
        PlacementViewFactory placement;
        BattleViewFactory battle;
    };
} // namespace cpp_warships::game_tui
