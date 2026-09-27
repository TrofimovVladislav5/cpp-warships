#pragma once

#include <application/core/Coordinate.h>
#include <application/head/Queries.h>
#include <application/head/input/EventHandler.h>

namespace cpp_warships::head {
    /** @brief Walks an aiming cursor around a board with the arrow keys, stopping at the edges.
     *  Every screen that aims at a board shares this one, whatever it is aiming for. */
    class MoveCursorEventHandler final : public EventHandler {
    public:
        MoveCursorEventHandler(core::Coordinate& cursor, BoardQuery board);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        core::Coordinate& cursor_;
        BoardQuery board_;
    };
} // namespace cpp_warships::head
