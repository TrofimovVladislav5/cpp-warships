#include <application/model/ApplicationContext.h>
#include <application/model/WarshipsGame.h>
#include <application/model/intents/GameIntent.h>
#include <application/model/intents/IntentProcessor.h>
#include <application/model/intents/IntentResult.h>
#include <application/model/scenarios/ScenarioQueue.h>
#include <application/model/scenarios/SequenceScenario.h>
#include <application/model/scenarios/SyncScenarioQueue.h>
#include <application/persistence/MemorySaveStorage.h>
#include <application/persistence/SaveArchive.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::model::scenarios {
    namespace {
        /** @brief An intent that notes the order it was applied in. */
        class OrderedIntent final : public intents::GameIntent {
        public:
            OrderedIntent(std::string label, std::vector<std::string>& record)
                : label_(std::move(label))
                , record_(record) {}

            [[nodiscard]] std::string name() const override {
                return label_;
            }

            [[nodiscard]] intents::IntentResult apply() const override {
                record_.push_back(label_);
                return intents::IntentResult::succeeded();
            }

        private:
            std::string label_;
            std::vector<std::string>& record_;
        };

        /** @brief A game, a context and a processor, kept alive together. */
        class ProcessorFixture {
        public:
            ProcessorFixture()
                : archive_(storage_)
                , game_(randomEngine_, archive_)
                , context_(game_)
                , processor_(context_) {}

            intents::IntentProcessor& processor() noexcept {
                return processor_;
            }

        private:
            flow::RandomEngine randomEngine_{55U};
            persistence::MemorySaveStorage storage_;
            persistence::SaveArchive archive_;
            WarshipsGame game_;
            ApplicationContext context_;
            intents::IntentProcessor processor_;
        };
    }  // namespace

    TEST(SyncScenarioQueueTests, StartsIdle) {
        ProcessorFixture fixture;
        const SyncScenarioQueue queue{fixture.processor()};

        EXPECT_TRUE(queue.isIdle());
    }

    TEST(SyncScenarioQueueTests, SubmittingLeavesSomethingWaiting) {
        ProcessorFixture fixture;
        SyncScenarioQueue queue{fixture.processor()};
        std::vector<std::string> record;

        queue.submit(scenarioOf("a thing", std::make_shared<OrderedIntent>("step", record)));

        EXPECT_FALSE(queue.isIdle());
        EXPECT_TRUE(record.empty());
    }

    TEST(SyncScenarioQueueTests, SubmittingNothingIsIgnored) {
        ProcessorFixture fixture;
        SyncScenarioQueue queue{fixture.processor()};

        queue.submit(nullptr);

        EXPECT_TRUE(queue.isIdle());
    }

    TEST(SyncScenarioQueueTests, StartingPlaysEverythingWaitingOut) {
        ProcessorFixture fixture;
        SyncScenarioQueue queue{fixture.processor()};
        std::vector<std::string> record;
        queue.submit(scenarioOf("first", std::make_shared<OrderedIntent>("first", record)));
        queue.submit(scenarioOf("second", std::make_shared<OrderedIntent>("second", record)));

        queue.start();

        EXPECT_EQ(record, (std::vector<std::string>{"first", "second"}));
        EXPECT_TRUE(queue.isIdle());
    }

    TEST(SyncScenarioQueueTests, StartingWithNothingWaitingIsHarmless) {
        ProcessorFixture fixture;
        SyncScenarioQueue queue{fixture.processor()};

        EXPECT_NO_THROW(queue.start());

        EXPECT_TRUE(queue.isIdle());
    }

    TEST(SyncScenarioQueueTests, JoiningReturnsAtOnceBecauseStartAlreadyFinished) {
        ProcessorFixture fixture;
        SyncScenarioQueue queue{fixture.processor()};
        std::vector<std::string> record;
        queue.submit(scenarioOf("a thing", std::make_shared<OrderedIntent>("step", record)));

        queue.start();
        queue.join();

        EXPECT_TRUE(queue.isIdle());
        EXPECT_EQ(record.size(), 1U);
    }

    TEST(SyncScenarioQueueTests, PlaysEveryStepOfEveryScenarioInOrder) {
        ProcessorFixture fixture;
        SyncScenarioQueue queue{fixture.processor()};
        std::vector<std::string> record;
        std::vector<intents::GameIntentPointer> steps;
        steps.push_back(std::make_shared<OrderedIntent>("one", record));
        steps.push_back(std::make_shared<OrderedIntent>("two", record));
        queue.submit(std::make_shared<SequenceScenario>("counting", std::move(steps)));

        queue.start();

        EXPECT_EQ(record, (std::vector<std::string>{"one", "two"}));
    }

    TEST(SyncScenarioQueueTests, IsUsableThroughTheQueueInterface) {
        ProcessorFixture fixture;
        SyncScenarioQueue concrete{fixture.processor()};
        ScenarioQueue& queue = concrete;
        std::vector<std::string> record;

        queue.submit(scenarioOf("a thing", std::make_shared<OrderedIntent>("step", record)));
        queue.start();
        queue.join();

        EXPECT_TRUE(queue.isIdle());
        EXPECT_EQ(record.size(), 1U);
    }
}  // namespace cpp_warships::model::scenarios
