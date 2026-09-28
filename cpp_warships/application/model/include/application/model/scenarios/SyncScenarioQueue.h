#pragma once

#include <application/model/scenarios/ScenarioQueue.h>

#include <deque>

namespace cpp_warships::model::intents {
    class IntentProcessor;
}

namespace cpp_warships::model::scenarios {
    /** @brief Plays scenarios out on the thread that asked for them. */
    class SyncScenarioQueue final : public ScenarioQueue {
    public:
        /** @brief Runs its scenarios through @p processor, which must outlive it. */
        explicit SyncScenarioQueue(intents::IntentProcessor& processor) noexcept;

        void submit(ScenarioPointer scenario) override;
        void start() override;
        void join() override;
        [[nodiscard]] bool isIdle() const override;

    private:
        intents::IntentProcessor& processor_;
        std::deque<ScenarioPointer> waiting_;
    };
}  // namespace cpp_warships::model::scenarios
