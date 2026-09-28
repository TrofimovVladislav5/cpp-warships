#include <build_game.h>

namespace cpp_warships::application {
    std::unique_ptr<model::WarshipsGame>
    buildGame(flow::RandomEngine& randomEngine, persistence::SaveArchive& saveArchive) {
        return std::make_unique<model::WarshipsGame>(randomEngine, saveArchive);
    }
}  // namespace cpp_warships::application
