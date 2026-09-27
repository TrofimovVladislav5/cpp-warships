#pragma once

#include <application/flow/RandomEngine.h>
#include <application/persistence/SaveArchive.h>
#include <application/head/session/Application.h>

namespace cpp_warships::application {
    /** @brief Opens a fresh session, played out with @p randomEngine.
     *  Anything a session is set up with, such as where saves live, is settled here. */
    [[nodiscard]] head::Application buildSession(
            flow::RandomEngine& randomEngine,
            persistence::SaveArchive& saveArchive
    );
} // namespace cpp_warships::application
