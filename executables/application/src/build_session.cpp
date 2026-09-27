#include <build_session.h>

namespace cpp_warships::application {
    game_tui::Application buildSession(game_flow::RandomEngine& randomEngine) {
        return game_tui::Application{randomEngine};
    }
} // namespace cpp_warships::application
