#include <build_queries.h>

namespace cpp_warships::application {
    SessionQueries
    buildQueries(const model::WarshipsGame& game, const head::ThemeSelection& theme) {
        return SessionQueries{
            .match = [&game]() -> const flow::Match& { return game.match(); },
            .hasMatch = [&game] { return game.hasMatch(); },
            .hasSavedMatch = [&game] { return game.saves().hasSavedMatch(); },
            .theme = [&theme]() -> const head::Theme& { return theme.current(); }
        };
    }
}  // namespace cpp_warships::application
