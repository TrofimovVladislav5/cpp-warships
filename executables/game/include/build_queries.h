#pragma once

#include <application/head/Queries.h>
#include <application/head/session/Application.h>

namespace cpp_warships::application {
    /** @brief The read-only questions a screen may ask about the session it is drawing. */
    struct SessionQueries {
        head::MatchQuery match;
        head::MatchInProgressQuery hasMatch;
        head::SavedMatchQuery hasSavedMatch;
        head::ThemeQuery theme;
    };

    /** @brief Questions that read @p session, which must outlive the answers. */
    [[nodiscard]] SessionQueries buildQueries(const head::Application& session);
} // namespace cpp_warships::application
