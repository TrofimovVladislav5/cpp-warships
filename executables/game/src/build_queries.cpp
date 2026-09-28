#include <application/head/common/ThemeSelection.h>
#include <application/model/WarshipsGame.h>
#include <build_queries.h>

namespace cpp_warships::application {
    SessionQueries buildQueries(
        const model::WarshipsGame& game,
        const head::common::ThemeSelection& theme
    ) {
        return SessionQueries{
            .match = [&game]() -> const flow::Match& { return game.match(); },
            .hasMatch = [&game] { return game.hasMatch(); },
            .hasSavedMatch = [&game] { return game.saves().hasSavedMatch(); },
            .nameInPlay = [&game] { return game.saves().nameInPlay(); },
            .theme = [&theme]() -> const head::common::Theme& { return theme.current(); }
        };
    }
}  // namespace cpp_warships::application
