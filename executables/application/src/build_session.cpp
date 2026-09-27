#include <build_session.h>

namespace cpp_warships::application {
    game_tui::Application buildSession(
            game_flow::RandomEngine& randomEngine,
            game_persistence::SaveArchive& saveArchive
    ) {
        return game_tui::Application{randomEngine, saveArchive};
    }
} // namespace cpp_warships::application
