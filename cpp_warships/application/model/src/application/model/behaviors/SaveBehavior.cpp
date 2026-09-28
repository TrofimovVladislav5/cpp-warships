#include <application/model/MatchInPlay.h>
#include <application/model/behaviors/SaveBehavior.h>
#include <application/persistence/MatchSnapshot.h>
#include <application/persistence/SaveArchive.h>

#include <optional>
#include <utility>

namespace cpp_warships::model::behaviors {
    SaveBehavior::SaveBehavior(MatchInPlay& inPlay, persistence::SaveArchive& saveArchive) noexcept
        : inPlay_(inPlay)
        , saveArchive_(saveArchive) {}

    bool SaveBehavior::hasSavedMatch() const {
        return !saveArchive_.listSaves().empty();
    }

    std::vector<persistence::SaveSummary> SaveBehavior::savedMatches() const {
        return saveArchive_.listSaves();
    }

    std::string SaveBehavior::nameInPlay() const {
        if (!inPlay_.loadedFrom().has_value()) {
            return {};
        }

        return saveArchive_.nameOf(*inPlay_.loadedFrom()).value_or(std::string{});
    }

    SaveOutcome SaveBehavior::saveMatch(const std::string& name) {
        if (!inPlay_.hasMatch()) {
            return SaveOutcome::NoMatchInPlay;
        }

        const flow::MatchEventLog story{
            inPlay_.journal().entries().begin(),
            inPlay_.journal().entries().end()
        };
        const std::string slot = inPlay_.loadedFrom().value_or(
            persistence::SaveArchive::newSaveId()
        );

        const auto matchSnapshot = persistence::MatchSnapshot::capture(inPlay_.match(), story);
        if (const bool isStored = saveArchive_.save(slot, name, matchSnapshot); !isStored) {
            return SaveOutcome::CouldNotWrite;
        }

        inPlay_.rememberSlot(slot);
        return SaveOutcome::Saved;
    }

    bool SaveBehavior::loadMatch(const std::string& name) {
        const std::optional<persistence::MatchSnapshot> saved = saveArchive_.load(name);
        if (!saved.has_value()) {
            return false;
        }

        inPlay_.replaceWith(saved->restore(inPlay_.randomEngine()), saved->journal(), name);
        return true;
    }

    bool SaveBehavior::deleteSave(const std::string& name) {
        return saveArchive_.remove(name);
    }
}  // namespace cpp_warships::model::behaviors
