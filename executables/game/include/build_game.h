#pragma once

#include <application/flow/RandomEngine.h>
#include <application/model/WarshipsGame.h>
#include <application/persistence/SaveArchive.h>

#include <memory>

namespace cpp_warships::application {
    /** @brief Opens a fresh game, played out with @p randomEngine and saved
     * into @p saveArchive, both of which must outlive it. */
    [[nodiscard]] std::unique_ptr<model::WarshipsGame>
    buildGame(flow::RandomEngine& randomEngine, persistence::SaveArchive& saveArchive);
}  // namespace cpp_warships::application
