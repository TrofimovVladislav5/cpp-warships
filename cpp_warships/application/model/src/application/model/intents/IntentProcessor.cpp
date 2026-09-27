#include <application/model/intents/IntentProcessor.h>

#include <exception>
#include <string>

#include <application/core/errors/WarshipsException.h>
#include <application/model/errors/ModelExceptions.h>

namespace cpp_warships::model {
    IntentProcessor::IntentProcessor(ApplicationContext& application) noexcept
        : application_(application) {}

    void IntentProcessor::run(IntendedGameScenario& scenario) {
        IntentResult previous = IntentResult::succeeded();
        int completedSteps = 0;

        while (const GameIntentPointer step = scenario.next(previous)) {
            previous = applyStep(*step);
            if (!previous.isSucceeded()) {
                application_.note(
                        ScenarioAbortedException(scenario.name(), completedSteps, previous.reason())
                                .what()
                );
                return;
            }

            ++completedSteps;
        }
    }

    IntentResult IntentProcessor::applyStep(const GameIntent& step) {
        // Everything the layers below can throw stops here. What travels on is a result,
        // so nothing above this has to know that exceptions were ever involved.
        try {
            return step.apply();
        } catch (const core::WarshipsException& error) {
            return IntentResult::failed(error.what());
        } catch (const std::exception& error) {
            return IntentResult::failed(std::string{"unexpectedly, "} + error.what());
        }
    }
} // namespace cpp_warships::model
