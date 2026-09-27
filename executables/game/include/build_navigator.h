#pragma once

#include <build_queries.h>

#include <application/head/intents/Intent.h>
#include <application/head/screens/ScreenNavigator.h>
#include <application/model/WarshipsGame.h>
#include <application/head/views/ViewFactory.h>

namespace cpp_warships::application {
    /** @brief A navigator holding every screen the game can show.
     *  This is the one place that names which screens exist and what each is given.
     *  @p session and @p queries must outlive the navigator, as the screens read them. */
    [[nodiscard]] head::ScreenNavigator buildNavigator(
            const head::IntentSink& intentSink,
            const model::WarshipsGame& game,
            const SessionQueries& queries,
            const head::ViewFactory& views
    );
} // namespace cpp_warships::application
