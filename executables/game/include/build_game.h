#pragma once

#include <memory>

#include <application/flow/RandomEngine.h>
#include <application/model/WarshipsGame.h>
#include <application/persistence/SaveArchive.h>

namespace cpp_warships::application {
    /** @brief Opens a fresh game, played out with @p randomEngine and saved into
     *  @p saveArchive, both of which must outlive it. Held behind a pointer because the
     *  game's behaviours point into its own state, so it cannot be copied or moved. */
    [[nodiscard]] std::unique_ptr<model::WarshipsGame> buildGame(
            flow::RandomEngine& randomEngine,
            persistence::SaveArchive& saveArchive
    );
} // namespace cpp_warships::application
