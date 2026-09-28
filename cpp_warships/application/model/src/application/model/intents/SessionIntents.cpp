#include <application/model/ApplicationContext.h>
#include <application/model/behaviors/SaveBehavior.h>
#include <application/model/intents/SessionIntents.h>

#include <utility>

namespace cpp_warships::model::intents {
    SaveMatchIntent::SaveMatchIntent(behaviors::SaveBehavior& saves, std::string name) noexcept
        : saves_(saves)
        , name_(std::move(name)) {}

    std::string SaveMatchIntent::name() const {
        return "saving the match";
    }

    IntentResult SaveMatchIntent::apply() const {
        switch (saves_.saveMatch(name_)) {
            case behaviors::SaveOutcome::Saved:
                return IntentResult::succeeded();
            case behaviors::SaveOutcome::NoMatchInPlay:
                return IntentResult::failed("there is no match to save");
            case behaviors::SaveOutcome::CouldNotWrite:
                return IntentResult::failed("the save could not be written");
        }

        return IntentResult::failed("the match could not be saved");
    }

    LoadMatchIntent::LoadMatchIntent(behaviors::SaveBehavior& saves, std::string slot) noexcept
        : saves_(saves)
        , slot_(std::move(slot)) {}

    std::string LoadMatchIntent::name() const {
        return "loading the match";
    }

    IntentResult LoadMatchIntent::apply() const {
        if (!saves_.loadMatch(slot_)) {
            return IntentResult::failed("that save could not be read");
        }

        return IntentResult::succeeded();
    }

    DeleteSaveIntent::DeleteSaveIntent(behaviors::SaveBehavior& saves, std::string slot) noexcept
        : saves_(saves)
        , slot_(std::move(slot)) {}

    std::string DeleteSaveIntent::name() const {
        return "deleting a save";
    }

    IntentResult DeleteSaveIntent::apply() const {
        if (!saves_.deleteSave(slot_)) {
            return IntentResult::failed("that save was already gone");
        }

        return IntentResult::succeeded();
    }

    FinishSessionIntent::FinishSessionIntent(ApplicationContext& application) noexcept
        : application_(application) {}

    std::string FinishSessionIntent::name() const {
        return "finishing the session";
    }

    IntentResult FinishSessionIntent::apply() const {
        application_.finish();
        return IntentResult::succeeded();
    }
}  // namespace cpp_warships::model::intents
