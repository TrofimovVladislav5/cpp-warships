#pragma once

#include <game_flow/RandomEngine.h>
#include <game_persistence/SaveArchive.h>
#include <game_tui/session/Application.h>

namespace cpp_warships::application {
    /** @brief Opens a fresh session, played out with @p randomEngine.
     *  Anything a session is set up with, such as where saves live, is settled here. */
    [[nodiscard]] game_tui::Application buildSession(
            game_flow::RandomEngine& randomEngine,
            game_persistence::SaveArchive& saveArchive
    );
} // namespace cpp_warships::application
