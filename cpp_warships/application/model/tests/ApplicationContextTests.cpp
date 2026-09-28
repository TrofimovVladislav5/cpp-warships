#include <application/model/ApplicationContext.h>
#include <application/model/WarshipsGame.h>
#include <application/persistence/MemorySaveStorage.h>
#include <application/persistence/SaveArchive.h>
#include <gtest/gtest.h>

#include <string>

namespace cpp_warships::model {
    namespace {
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

            WarshipsGame& game() noexcept {
                return game_;
            }

        private:
            flow::RandomEngine randomEngine_{1234U};
            persistence::MemorySaveStorage storage_;
            persistence::SaveArchive archive_;
            WarshipsGame game_;
            ApplicationContext context_;
        };
    }  // namespace

    TEST(ApplicationContextTests, StartsUnfinishedAndQuiet) {
        GameFixture fixture;

        EXPECT_FALSE(fixture.context().isFinished());
        EXPECT_TRUE(fixture.context().notices().empty());
    }

    TEST(ApplicationContextTests, PointsAtTheGameItWasBuiltOver) {
        GameFixture fixture;

        EXPECT_EQ(&fixture.context().game(), &fixture.game());
    }

    TEST(ApplicationContextTests, ReadsTheGameThroughAConstContextToo) {
        GameFixture fixture;
        const ApplicationContext& readOnly = fixture.context();

        EXPECT_FALSE(readOnly.game().hasMatch());
    }

    TEST(ApplicationContextTests, FinishingIsRemembered) {
        GameFixture fixture;

        fixture.context().finish();

        EXPECT_TRUE(fixture.context().isFinished());
    }

    TEST(ApplicationContextTests, FinishingTwiceIsStillFinished) {
        GameFixture fixture;

        fixture.context().finish();
        fixture.context().finish();

        EXPECT_TRUE(fixture.context().isFinished());
    }

    TEST(ApplicationContextTests, KeepsNoticesOldestFirst) {
        GameFixture fixture;

        fixture.context().note("first");
        fixture.context().note("second");

        ASSERT_EQ(fixture.context().notices().size(), 2U);
        EXPECT_EQ(fixture.context().notices().front(), "first");
        EXPECT_EQ(fixture.context().notices().back(), "second");
    }

    TEST(ApplicationContextTests, KeepsOnlyTheLastFewNotices) {
        GameFixture fixture;

        for (int index = 0; index < 12; ++index) {
            fixture.context().note("notice " + std::to_string(index));
        }

        EXPECT_EQ(fixture.context().notices().size(), 8U);
        EXPECT_EQ(fixture.context().notices().back(), "notice 11");
        EXPECT_EQ(fixture.context().notices().front(), "notice 4");
    }

    TEST(ApplicationContextTests, ClearingSilencesTheNotices) {
        GameFixture fixture;
        fixture.context().note("something");

        fixture.context().clearNotices();

        EXPECT_TRUE(fixture.context().notices().empty());
    }

    TEST(ApplicationContextTests, ClearingLeavesTheSessionRunning) {
        GameFixture fixture;
        fixture.context().note("something");

        fixture.context().clearNotices();

        EXPECT_FALSE(fixture.context().isFinished());
    }
}  // namespace cpp_warships::model
