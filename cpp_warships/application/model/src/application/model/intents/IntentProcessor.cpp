#include <application/core/errors/WarshipsException.h>
#include <application/model/intents/IntentProcessor.h>

#include <exception>
#include <string>

namespace cpp_warships::model {
    IntentProcessor::IntentProcessor(ApplicationContext& application) noexcept
        : application_(application) {
    }

    void IntentProcessor::run(IntendedGameScenario& scenario) {
        application_.clearNotices();

        IntentResult previous = IntentResult::succeeded();
        int completedSteps = 0;

        while (const GameIntentPointer step = scenario.next(previous)) {
            previous = applyStep(*step);
            if (!previous.isSucceeded()) {
                application_.note(
                    scenario.name() + " stopped after " + std::to_string(completedSteps) +
                    " of its steps: " + previous.reason()
                );
                return;
            }

            ++completedSteps;
        }
    }

    IntentResult IntentProcessor::applyStep(const GameIntent& step) {
        try {
            return step.apply();
        } catch (const core::WarshipsException& error) {
            return IntentResult::failed(error.what());
        } catch (const std::exception& error) {
            return IntentResult::failed(std::string{"unexpectedly, "} + error.what());
        }
    }
}  // namespace cpp_warships::model
