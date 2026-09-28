#include <application/model/ApplicationContext.h>
#include <application/model/behaviors/SaveBehavior.h>
#include <application/model/intents/SessionIntents.h>

namespace cpp_warships::model::intents {
    SaveMatchIntent::SaveMatchIntent(behaviors::SaveBehavior& saves) noexcept
        : saves_(saves) {}

    std::string SaveMatchIntent::name() const {
        return "saving the match";
    }

    IntentResult SaveMatchIntent::apply() const {
        switch (saves_.saveMatch()) {
            case behaviors::SaveOutcome::Saved:
                return IntentResult::succeeded();
            case behaviors::SaveOutcome::NoMatchInPlay:
                return IntentResult::failed("there is no match to save");
            case behaviors::SaveOutcome::CouldNotWrite:
                return IntentResult::failed("the save could not be written");
        }

        return IntentResult::failed("the match could not be saved");
    }

    LoadMatchIntent::LoadMatchIntent(behaviors::SaveBehavior& saves) noexcept
        : saves_(saves) {}

    std::string LoadMatchIntent::name() const {
        return "loading the match";
    }

    IntentResult LoadMatchIntent::apply() const {
        if (!saves_.loadMatch()) {
            return IntentResult::failed("there is nothing saved to load");
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
