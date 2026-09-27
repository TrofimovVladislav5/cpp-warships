#include <application/model/scenarios/SequenceScenario.h>

#include <memory>
#include <utility>

namespace cpp_warships::model {
    SequenceScenario::SequenceScenario(std::string name, std::vector<GameIntentPointer> steps)
        : name_(std::move(name))
        , steps_(std::move(steps)) {}

    std::string SequenceScenario::name() const {
        return name_;
    }

    GameIntentPointer SequenceScenario::next(const IntentResult& previous) {
        if (!previous.isSucceeded() || taken_ >= steps_.size()) {
            return nullptr;
        }

        return steps_[taken_++];
    }

    ScenarioPointer scenarioOf(std::string name, GameIntentPointer step) {
        std::vector<GameIntentPointer> steps;
        steps.push_back(std::move(step));

        return std::make_shared<SequenceScenario>(std::move(name), std::move(steps));
    }
} // namespace cpp_warships::model
