#include <application/model/scenarios/SyncScenarioQueue.h>

#include <utility>

namespace cpp_warships::model {
    SyncScenarioQueue::SyncScenarioQueue(IntentProcessor& processor) noexcept
        : processor_(processor) {}

    void SyncScenarioQueue::submit(ScenarioPointer scenario) {
        if (scenario != nullptr) {
            waiting_.push_back(std::move(scenario));
        }
    }

    void SyncScenarioQueue::start() {
        // Taken one at a time rather than drained, because playing one out may add
        // another, and that one belongs to this same run.
        while (!waiting_.empty()) {
            const ScenarioPointer scenario = waiting_.front();
            waiting_.pop_front();
            processor_.run(*scenario);
        }
    }

    void SyncScenarioQueue::join() {
        // Nothing to wait for: start already played everything out on this thread.
    }

    bool SyncScenarioQueue::isIdle() const {
        return waiting_.empty();
    }
} // namespace cpp_warships::model
