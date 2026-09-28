#include <application/model/scenarios/SequenceScenario.h>

#include <memory>
#include <utility>

namespace cpp_warships::model::scenarios {
    SequenceScenario::SequenceScenario(
        std::string name,
        std::vector<intents::GameIntentPointer> steps
    )
        : name_(std::move(name))
        , steps_(std::move(steps)) {}

    std::string SequenceScenario::name() const {
        return name_;
    }

    intents::GameIntentPointer SequenceScenario::next(const intents::IntentResult& previous) {
        if (!previous.isSucceeded() || taken_ >= steps_.size()) {
            return nullptr;
        }

        return steps_[taken_++];
    }

    ScenarioPointer scenarioOf(std::string name, intents::GameIntentPointer step) {
        std::vector<intents::GameIntentPointer> steps;
        steps.push_back(std::move(step));

        return std::make_shared<SequenceScenario>(std::move(name), std::move(steps));
    }
}  // namespace cpp_warships::model::scenarios
