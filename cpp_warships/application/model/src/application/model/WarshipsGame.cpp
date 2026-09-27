#include <application/model/WarshipsGame.h>

namespace cpp_warships::model {
    WarshipsGame::WarshipsGame(
            flow::RandomEngine& randomEngine,
            persistence::SaveArchive& saveArchive
    ) noexcept
        : inPlay_(randomEngine)
        , play_(inPlay_)
        , saves_(inPlay_, saveArchive) {}

    bool WarshipsGame::hasMatch() const noexcept {
        return inPlay_.hasMatch();
    }

    const flow::Match& WarshipsGame::match() const {
        return inPlay_.match();
    }

    const BattleJournal& WarshipsGame::journal() const noexcept {
        return inPlay_.journal();
    }

    MatchBehavior& WarshipsGame::play() noexcept {
        return play_;
    }

    SaveBehavior& WarshipsGame::saves() noexcept {
        return saves_;
    }

    const SaveBehavior& WarshipsGame::saves() const noexcept {
        return saves_;
    }
} // namespace cpp_warships::model
