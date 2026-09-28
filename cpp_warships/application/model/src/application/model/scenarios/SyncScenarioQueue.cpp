#include <application/model/scenarios/SyncScenarioQueue.h>

#include <utility>

namespace cpp_warships::model {
    SyncScenarioQueue::SyncScenarioQueue(IntentProcessor& processor) noexcept
        : processor_(processor) {
    }

    void SyncScenarioQueue::submit(ScenarioPointer scenario) {
        if (scenario != nullptr) {
            waiting_.push_back(std::move(scenario));
        }
    }

    void SyncScenarioQueue::start() {
        while (!waiting_.empty()) {
            const ScenarioPointer scenario = waiting_.front();
            waiting_.pop_front();
            processor_.run(*scenario);
        }
    }

    void SyncScenarioQueue::join() {
    }

    bool SyncScenarioQueue::isIdle() const {
        return waiting_.empty();
    }
}  // namespace cpp_warships::model
