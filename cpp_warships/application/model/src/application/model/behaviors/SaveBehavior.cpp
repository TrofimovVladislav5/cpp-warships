#include <application/model/behaviors/SaveBehavior.h>

#include <optional>

#include <application/persistence/MatchSnapshot.h>

namespace cpp_warships::model {
    namespace {
        /** @brief The one slot a session saves into. Several would want a screen to pick from. */
        const std::string SAVE_SLOT_NAME = "quicksave";
    } // namespace

    SaveBehavior::SaveBehavior(
            MatchInPlay& inPlay,
            persistence::SaveArchive& saveArchive
    ) noexcept
        : inPlay_(inPlay)
        , saveArchive_(saveArchive) {}

    bool SaveBehavior::hasSavedMatch() const {
        return saveArchive_.load(SAVE_SLOT_NAME).has_value();
    }

    bool SaveBehavior::saveMatch() {
        if (!inPlay_.hasMatch()) {
            return false;
        }

        return saveArchive_.save(
                SAVE_SLOT_NAME,
                persistence::MatchSnapshot::capture(inPlay_.match())
        );
    }

    bool SaveBehavior::loadMatch() {
        const std::optional<persistence::MatchSnapshot> saved =
                saveArchive_.load(SAVE_SLOT_NAME);
        if (!saved.has_value()) {
            return false;
        }

        inPlay_.replaceWith(saved->restore(inPlay_.randomEngine()));
        return true;
    }
} // namespace cpp_warships::model
