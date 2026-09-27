#include <build_queries.h>

namespace cpp_warships::application {
    SessionQueries buildQueries(const game_tui::Application& session) {
        return SessionQueries{
                .match = [&session]() -> const game_flow::Match& {
                    return session.match();
                },
                .hasMatch =
                        [&session] {
                            return session.hasMatch();
                        },
                .theme = [&session]() -> const game_tui::Theme& {
                    return session.theme();
                }
        };
    }
} // namespace cpp_warships::application
