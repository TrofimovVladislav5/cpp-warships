#include <application/model/behaviors/SaveBehavior.h>
#include <application/persistence/MatchSnapshot.h>

#include <optional>

namespace cpp_warships::model {
    namespace {
        /** @brief The one slot a session saves into. Several would want a
         * screen to pick from. */
        const std::string SAVE_SLOT_NAME = "quicksave";
    }  // namespace

    SaveBehavior::SaveBehavior(MatchInPlay& inPlay, persistence::SaveArchive& saveArchive) noexcept
        : inPlay_(inPlay), saveArchive_(saveArchive) {
    }

    bool SaveBehavior::hasSavedMatch() const {
        return saveArchive_.load(SAVE_SLOT_NAME).has_value();
    }

    SaveOutcome SaveBehavior::saveMatch() {
        if (!inPlay_.hasMatch()) {
            return SaveOutcome::NoMatchInPlay;
        }

        const flow::MatchEventLog story{
            inPlay_.journal().entries().begin(),
            inPlay_.journal().entries().end()
        };

        const bool isStored = saveArchive_.save(
            SAVE_SLOT_NAME,
            persistence::MatchSnapshot::capture(inPlay_.match(), story)
        );

        return isStored ? SaveOutcome::Saved : SaveOutcome::CouldNotWrite;
    }

    bool SaveBehavior::loadMatch() {
        const std::optional<persistence::MatchSnapshot> saved = saveArchive_.load(SAVE_SLOT_NAME);
        if (!saved.has_value()) {
            return false;
        }

        inPlay_.replaceWith(saved->restore(inPlay_.randomEngine()), saved->journal());
        return true;
    }
}  // namespace cpp_warships::model
