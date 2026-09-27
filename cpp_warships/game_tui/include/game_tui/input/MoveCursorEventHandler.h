#pragma once

#include <game_core/Coordinate.h>
#include <game_tui/Queries.h>
#include <game_tui/input/EventHandler.h>

namespace cpp_warships::game_tui {
    /** @brief Walks an aiming cursor around a board with the arrow keys, stopping at the edges.
     *  Every screen that aims at a board shares this one, whatever it is aiming for. */
    class MoveCursorEventHandler final : public EventHandler {
    public:
        MoveCursorEventHandler(game_core::Coordinate& cursor, BoardQuery board);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        game_core::Coordinate& cursor_;
        BoardQuery board_;
    };
} // namespace cpp_warships::game_tui
