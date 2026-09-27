#include <build_queries.h>

namespace cpp_warships::application {
    SessionQueries buildQueries(const head::Application& session) {
        return SessionQueries{
                .match = [&session]() -> const flow::Match& {
                    return session.match();
                },
                .hasMatch =
                        [&session] {
                            return session.hasMatch();
                        },
                .hasSavedMatch =
                        [&session] {
                            return session.hasSavedMatch();
                        },
                .theme = [&session]() -> const head::Theme& {
                    return session.theme();
                }
        };
    }
} // namespace cpp_warships::application
