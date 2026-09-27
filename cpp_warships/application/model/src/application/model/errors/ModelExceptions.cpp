#include <application/model/errors/ModelExceptions.h>

namespace cpp_warships::model {
    ModelException::ModelException(const std::string& message)
        : WarshipsException(core::ErrorLayer::Model, message) {}

    NoMatchInPlayException::NoMatchInPlayException()
        : ModelException("there is no match in play") {}

    IntentFailedException::IntentFailedException(
            const std::string& intentName,
            const std::string& cause
    )
        : ModelException(intentName + " could not be carried out: " + cause)
        , intentName_(intentName)
        , cause_(cause) {}

    const std::string& IntentFailedException::intentName() const noexcept {
        return intentName_;
    }

    const std::string& IntentFailedException::cause() const noexcept {
        return cause_;
    }

    ScenarioAbortedException::ScenarioAbortedException(
            const std::string& scenarioName,
            const int completedSteps,
            const std::string& cause
    )
        : ModelException(
                  scenarioName + " stopped after " + std::to_string(completedSteps) +
                  " of its steps: " + cause
          )
        , scenarioName_(scenarioName)
        , completedSteps_(completedSteps)
        , cause_(cause) {}

    const std::string& ScenarioAbortedException::scenarioName() const noexcept {
        return scenarioName_;
    }

    int ScenarioAbortedException::completedSteps() const noexcept {
        return completedSteps_;
    }

    const std::string& ScenarioAbortedException::cause() const noexcept {
        return cause_;
    }
} // namespace cpp_warships::model
