#include <application/core/errors/ErrorLayer.h>
#include <application/model/ApplicationContext.h>
#include <application/model/WarshipsGame.h>
#include <application/model/errors/ModelExceptions.h>
#include <application/model/intents/GameIntent.h>
#include <application/model/intents/IntentFactory.h>
#include <application/model/intents/IntentProcessor.h>
#include <application/model/intents/IntentResult.h>
#include <application/model/scenarios/IntendedGameScenario.h>
#include <application/model/scenarios/SequenceScenario.h>
#include <application/persistence/MemorySaveStorage.h>
#include <application/persistence/SaveArchive.h>
#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::model::intents {
    namespace {
        /** @brief An intent that reports what it was told to, and counts its applications. */
        class ScriptedIntent final : public GameIntent {
        public:
            ScriptedIntent(std::string label, IntentResult outcome)
                : label_(std::move(label))
                , outcome_(std::move(outcome)) {}

            [[nodiscard]] std::string name() const override {
                return label_;
            }

            [[nodiscard]] IntentResult apply() const override {
                ++appliedCount;
                return outcome_;
            }

            mutable int appliedCount = 0;

        private:
            std::string label_;
            IntentResult outcome_;
        };

        /** @brief An intent that throws rather than reporting. */
        class ThrowingIntent final : public GameIntent {
        public:
            explicit ThrowingIntent(bool isFromTheGame)
                : isFromTheGame_(isFromTheGame) {}

            [[nodiscard]] std::string name() const override {
                return "throwing";
            }

            [[nodiscard]] IntentResult apply() const override {
                if (isFromTheGame_) {
                    throw errors::NoMatchInPlayException();
                }

                throw std::runtime_error("something else went wrong");
            }

        private:
            bool isFromTheGame_;
        };

        /** @brief A game and everything it needs, kept alive together. */
        class GameFixture {
        public:
            GameFixture()
                : archive_(storage_)
                , game_(randomEngine_, archive_)
                , context_(game_) {}

            ApplicationContext& context() noexcept {
                return context_;
            }

        private:
            flow::RandomEngine randomEngine_{99U};
            persistence::MemorySaveStorage storage_;
            persistence::SaveArchive archive_;
            WarshipsGame game_;
            ApplicationContext context_;
        };
    }  // namespace

    TEST(IntentResultTests, ASuccessCarriesNoReason) {
        const IntentResult result = IntentResult::succeeded();

        EXPECT_TRUE(result.isSucceeded());
        EXPECT_TRUE(result.reason().empty());
    }

    TEST(IntentResultTests, AFailureCarriesItsReason) {
        const IntentResult result = IntentResult::failed("no match in play");

        EXPECT_FALSE(result.isSucceeded());
        EXPECT_EQ(result.reason(), "no match in play");
    }

    TEST(SequenceScenarioTests, KeepsTheNameItWasGiven) {
        const scenarios::SequenceScenario scenario{"doing a thing", {}};

        EXPECT_EQ(scenario.name(), "doing a thing");
    }

    TEST(SequenceScenarioTests, AnEmptySequenceHasNoSteps) {
        scenarios::SequenceScenario scenario{"nothing", {}};

        EXPECT_EQ(scenario.next(IntentResult::succeeded()), nullptr);
    }

    TEST(SequenceScenarioTests, HandsOutItsStepsInOrder) {
        const auto first = std::make_shared<ScriptedIntent>("first", IntentResult::succeeded());
        const auto second = std::make_shared<ScriptedIntent>("second", IntentResult::succeeded());
        scenarios::SequenceScenario scenario{"two steps", {first, second}};

        EXPECT_EQ(scenario.next(IntentResult::succeeded()), first);
        EXPECT_EQ(scenario.next(IntentResult::succeeded()), second);
        EXPECT_EQ(scenario.next(IntentResult::succeeded()), nullptr);
    }

    TEST(SequenceScenarioTests, StopsAtTheFirstStepThatDidNotComeOff) {
        const auto first = std::make_shared<ScriptedIntent>("first", IntentResult::succeeded());
        const auto second = std::make_shared<ScriptedIntent>("second", IntentResult::succeeded());
        scenarios::SequenceScenario scenario{"two steps", {first, second}};
        (void)scenario.next(IntentResult::succeeded());

        EXPECT_EQ(scenario.next(IntentResult::failed("no")), nullptr);
    }

    TEST(SequenceScenarioTests, AScenarioOfOneStepWrapsThatStep) {
        const auto only = std::make_shared<ScriptedIntent>("only", IntentResult::succeeded());

        const scenarios::ScenarioPointer scenario = scenarios::scenarioOf("just one", only);

        ASSERT_NE(scenario, nullptr);
        EXPECT_EQ(scenario->name(), "just one");
        EXPECT_EQ(scenario->next(IntentResult::succeeded()), only);
        EXPECT_EQ(scenario->next(IntentResult::succeeded()), nullptr);
    }

    TEST(IntentProcessorTests, TakesEveryStepOfAScenarioThatComesOff) {
        GameFixture fixture;
        IntentProcessor processor{fixture.context()};
        const auto first = std::make_shared<ScriptedIntent>("first", IntentResult::succeeded());
        const auto second = std::make_shared<ScriptedIntent>("second", IntentResult::succeeded());
        scenarios::SequenceScenario scenario{"two steps", {first, second}};

        processor.run(scenario);

        EXPECT_EQ(first->appliedCount, 1);
        EXPECT_EQ(second->appliedCount, 1);
        EXPECT_TRUE(fixture.context().notices().empty());
    }

    TEST(IntentProcessorTests, StopsAtAStepThatDidNotComeOff) {
        GameFixture fixture;
        IntentProcessor processor{fixture.context()};
        const auto first = std::make_shared<ScriptedIntent>("first", IntentResult::failed("no"));
        const auto second = std::make_shared<ScriptedIntent>("second", IntentResult::succeeded());
        scenarios::SequenceScenario scenario{"two steps", {first, second}};

        processor.run(scenario);

        EXPECT_EQ(first->appliedCount, 1);
        EXPECT_EQ(second->appliedCount, 0);
    }

    TEST(IntentProcessorTests, SaysWhichScenarioStoppedAndWhere) {
        GameFixture fixture;
        IntentProcessor processor{fixture.context()};
        const auto first = std::make_shared<ScriptedIntent>("first", IntentResult::succeeded());
        const auto second =
            std::make_shared<ScriptedIntent>("second", IntentResult::failed("the reason"));
        scenarios::SequenceScenario scenario{"doing a thing", {first, second}};

        processor.run(scenario);

        ASSERT_EQ(fixture.context().notices().size(), 1U);
        const std::string& notice = fixture.context().notices().front();
        EXPECT_NE(notice.find("doing a thing"), std::string::npos);
        EXPECT_NE(notice.find("1 of its steps"), std::string::npos);
        EXPECT_NE(notice.find("the reason"), std::string::npos);
    }

    TEST(IntentProcessorTests, TurnsAnErrorFromTheGameIntoAFailedStep) {
        GameFixture fixture;
        IntentProcessor processor{fixture.context()};
        scenarios::SequenceScenario scenario{"throwing", {std::make_shared<ThrowingIntent>(true)}};

        EXPECT_NO_THROW(processor.run(scenario));

        ASSERT_EQ(fixture.context().notices().size(), 1U);
        EXPECT_NE(fixture.context().notices().front().find("model error"), std::string::npos);
    }

    TEST(IntentProcessorTests, TurnsAnyOtherErrorIntoAFailedStepToo) {
        GameFixture fixture;
        IntentProcessor processor{fixture.context()};
        scenarios::SequenceScenario scenario{"throwing", {std::make_shared<ThrowingIntent>(false)}};

        EXPECT_NO_THROW(processor.run(scenario));

        ASSERT_EQ(fixture.context().notices().size(), 1U);
        EXPECT_NE(fixture.context().notices().front().find("unexpectedly"), std::string::npos);
    }

    TEST(IntentProcessorTests, ClearsWhatWasSaidBeforeRunning) {
        GameFixture fixture;
        IntentProcessor processor{fixture.context()};
        fixture.context().note("something older");
        scenarios::SequenceScenario scenario{"nothing", {}};

        processor.run(scenario);

        EXPECT_TRUE(fixture.context().notices().empty());
    }

    TEST(IntentFactoryTests, BuildsEveryKindOfIntent) {
        GameFixture fixture;
        const IntentFactory factory{fixture.context()};

        EXPECT_NE(factory.startMatch(10), nullptr);
        EXPECT_NE(factory.placeShip({0, 0}, core::Direction::Horizontal, 2), nullptr);
        EXPECT_NE(factory.removeShip({0, 0}), nullptr);
        EXPECT_NE(factory.shuffleFleet(), nullptr);
        EXPECT_NE(factory.beginBattle(), nullptr);
        EXPECT_NE(factory.fireAt({1, 1}), nullptr);
        EXPECT_NE(factory.useSkill(std::nullopt), nullptr);
        EXPECT_NE(factory.saveMatch("a name"), nullptr);
        EXPECT_NE(factory.loadMatch("a slot"), nullptr);
        EXPECT_NE(factory.deleteSave("a slot"), nullptr);
        EXPECT_NE(factory.finishSession(), nullptr);
    }

    TEST(IntentFactoryTests, EveryIntentSaysWhatItIs) {
        GameFixture fixture;
        const IntentFactory factory{fixture.context()};

        EXPECT_EQ(factory.startMatch(10)->name(), "starting a match");
        EXPECT_EQ(factory.shuffleFleet()->name(), "shuffling the fleet");
        EXPECT_EQ(factory.beginBattle()->name(), "opening fire");
        EXPECT_EQ(factory.saveMatch("a name")->name(), "saving the match");
        EXPECT_EQ(factory.loadMatch("a slot")->name(), "loading the match");
        EXPECT_EQ(factory.deleteSave("a slot")->name(), "deleting a save");
        EXPECT_EQ(factory.finishSession()->name(), "finishing the session");
    }

    TEST(IntentFactoryTests, FinishingTheSessionSaysSoOnTheContext) {
        GameFixture fixture;
        const IntentFactory factory{fixture.context()};

        EXPECT_TRUE(factory.finishSession()->apply().isSucceeded());

        EXPECT_TRUE(fixture.context().isFinished());
    }

    TEST(IntentFactoryTests, StartingAMatchPutsOneInPlay) {
        GameFixture fixture;
        const IntentFactory factory{fixture.context()};

        EXPECT_TRUE(factory.startMatch(10)->apply().isSucceeded());

        EXPECT_TRUE(fixture.context().game().hasMatch());
    }

    TEST(IntentFactoryTests, SavingWithNoMatchInPlayFails) {
        GameFixture fixture;
        const IntentFactory factory{fixture.context()};

        const IntentResult result = factory.saveMatch("a name")->apply();

        EXPECT_FALSE(result.isSucceeded());
        EXPECT_EQ(result.reason(), "there is no match to save");
    }

    TEST(IntentFactoryTests, LoadingASaveThatIsNotThereFails) {
        GameFixture fixture;
        const IntentFactory factory{fixture.context()};

        const IntentResult result = factory.loadMatch("missing")->apply();

        EXPECT_FALSE(result.isSucceeded());
        EXPECT_EQ(result.reason(), "that save could not be read");
    }

    TEST(IntentFactoryTests, DeletingASaveThatIsNotThereFails) {
        GameFixture fixture;
        const IntentFactory factory{fixture.context()};

        const IntentResult result = factory.deleteSave("missing")->apply();

        EXPECT_FALSE(result.isSucceeded());
        EXPECT_EQ(result.reason(), "that save was already gone");
    }

    TEST(ModelExceptionTests, NoMatchInPlayComesFromTheModelLayer) {
        const errors::NoMatchInPlayException error;

        EXPECT_EQ(error.layer(), core::errors::ErrorLayer::Model);
        EXPECT_NE(std::string{error.what()}.find("model error"), std::string::npos);
    }

    TEST(ModelExceptionTests, IsCaughtAsAWarshipsException) {
        try {
            throw errors::NoMatchInPlayException();
        } catch (const core::errors::WarshipsException& error) {
            EXPECT_EQ(error.layer(), core::errors::ErrorLayer::Model);
            return;
        }

        FAIL() << "the exception was not thrown";
    }
}  // namespace cpp_warships::model::intents
