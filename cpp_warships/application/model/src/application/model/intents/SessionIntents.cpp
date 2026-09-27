#include <application/model/intents/SessionIntents.h>

namespace cpp_warships::model {
    SaveMatchIntent::SaveMatchIntent(SaveBehavior& saves) noexcept
        : saves_(saves) {}

    std::string SaveMatchIntent::name() const {
        return "saving the match";
    }

    IntentResult SaveMatchIntent::apply() const {
        if (!saves_.saveMatch()) {
            return IntentResult::failed("there is no match to save");
        }

        return IntentResult::succeeded();
    }

    LoadMatchIntent::LoadMatchIntent(SaveBehavior& saves) noexcept
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
} // namespace cpp_warships::model
