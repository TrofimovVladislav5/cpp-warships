#pragma once

namespace cpp_warships::flow {
    /** @brief Which part of a match is being played. Kept apart from the match itself so that
     * naming a phase costs nothing: an error may say it without pulling the rules in. */
    enum class MatchPhase {
        Placement,
        Battle,
        Finished,
    };
}  // namespace cpp_warships::flow
