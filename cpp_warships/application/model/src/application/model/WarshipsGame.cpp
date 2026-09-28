#include <application/flow/Match.h>
#include <application/model/BattleJournal.h>
#include <application/model/WarshipsGame.h>
#include <application/persistence/SaveArchive.h>

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

    behaviors::MatchBehavior& WarshipsGame::play() noexcept {
        return play_;
    }

    behaviors::SaveBehavior& WarshipsGame::saves() noexcept {
        return saves_;
    }

    const behaviors::SaveBehavior& WarshipsGame::saves() const noexcept {
        return saves_;
    }
}  // namespace cpp_warships::model
