#pragma once

#include <deque>

#include <application/model/intents/IntentProcessor.h>
#include <application/model/scenarios/ScenarioQueue.h>

namespace cpp_warships::model {
    /** @brief Plays scenarios out on the thread that asked for them. Starting drains the
     *  queue there and then, so joining has nothing left to wait for and returns at once.
     *  The two are still kept apart, because a queue that works elsewhere needs both. */
    class SyncScenarioQueue final : public ScenarioQueue {
    public:
        /** @brief Runs its scenarios through @p processor, which must outlive it. */
        explicit SyncScenarioQueue(IntentProcessor& processor) noexcept;

        void submit(ScenarioPointer scenario) override;
        void start() override;
        void join() override;
        [[nodiscard]] bool isIdle() const override;

    private:
        IntentProcessor& processor_;
        std::deque<ScenarioPointer> waiting_;
    };
} // namespace cpp_warships::model
