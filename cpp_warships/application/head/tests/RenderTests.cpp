#include <application/core/Coordinate.h>
#include <application/flow/MatchEvent.h>
#include <application/flow/SkillKind.h>
#include <application/head/common/Theme.h>
#include <application/head/common/render/CoordinateLabel.h>
#include <application/head/common/render/EventNarration.h>
#include <application/head/common/render/Notices.h>
#include <application/head/common/state/BattleState.h>
#include <application/model/ApplicationContext.h>
#include <application/model/WarshipsGame.h>
#include <application/persistence/MemorySaveStorage.h>
#include <application/persistence/SaveArchive.h>
#include <gtest/gtest.h>

#include <string>
#include <unordered_set>
#include <vector>

namespace cpp_warships::head::common::render {
    namespace {
        /** @brief A context over a game, all kept alive together. */
        class ContextFixture {
        public:
            ContextFixture()
                : archive_(storage_)
                , game_(randomEngine_, archive_)
                , context_(game_) {}

            model::ApplicationContext& context() noexcept {
                return context_;
            }

        private:
            flow::RandomEngine randomEngine_{3U};
            persistence::MemorySaveStorage storage_;
            persistence::SaveArchive archive_;
            model::WarshipsGame game_;
            model::ApplicationContext context_;
        };
    }  // namespace

    TEST(CoordinateLabelTests, HeadsColumnsWithLetters) {
        EXPECT_EQ(columnLabel(0), "A");
        EXPECT_EQ(columnLabel(1), "B");
        EXPECT_EQ(columnLabel(25), "Z");
    }

    TEST(CoordinateLabelTests, MarksAColumnPastTheAlphabetAsUnknown) {
        EXPECT_EQ(columnLabel(26), "?");
        EXPECT_EQ(columnLabel(-1), "?");
    }

    TEST(CoordinateLabelTests, WritesACellAsTheGridLabelsIt) {
        EXPECT_EQ(coordinateLabel({0, 0}), "A1");
        EXPECT_EQ(coordinateLabel({2, 4}), "C5");
        EXPECT_EQ(coordinateLabel({9, 9}), "J10");
    }

    TEST(EventNarrationTests, NamesEverySkillForThePlayer) {
        for (const flow::SkillKind skill : flow::ALL_SKILL_KINDS) {
            EXPECT_FALSE(skillName(skill).empty());
        }

        EXPECT_EQ(skillName(flow::SkillKind::Scanner), "scanner");
        EXPECT_EQ(skillName(flow::SkillKind::DoubleDamage), "double damage");
        EXPECT_EQ(skillName(flow::SkillKind::RandomStrike), "random strike");
    }

    TEST(EventNarrationTests, GivesEveryKindOfEventALine) {
        const Theme& theme = defaultTheme();
        const flow::MatchEventKind kinds[]{
            flow::MatchEventKind::ShotMissed,
            flow::MatchEventKind::ShipDamaged,
            flow::MatchEventKind::ShipSunk,
            flow::MatchEventKind::ShotRejected,
            flow::MatchEventKind::SkillGranted,
            flow::MatchEventKind::DoubleDamageArmed,
            flow::MatchEventKind::AreaScanned,
            flow::MatchEventKind::RoundWon,
            flow::MatchEventKind::MatchLost,
            flow::MatchEventKind::TurnPassed,
        };

        for (const flow::MatchEventKind kind : kinds) {
            const flow::MatchEvent event{
                .kind = kind,
                .coordinate = core::Coordinate{1, 1},
                .skill = flow::SkillKind::Scanner
            };

            EXPECT_FALSE(narrate(event, theme).text.empty());
        }
    }

    TEST(EventNarrationTests, SaysWhoDidIt) {
        const Theme& theme = defaultTheme();
        const flow::MatchEvent byPlayer{
            .kind = flow::MatchEventKind::ShotMissed,
            .actor = flow::Participant::Player,
            .coordinate = core::Coordinate{0, 0}
        };
        const flow::MatchEvent byComputer{
            .kind = flow::MatchEventKind::ShotMissed,
            .actor = flow::Participant::Computer,
            .coordinate = core::Coordinate{0, 0}
        };

        EXPECT_NE(narrate(byPlayer, theme).text, narrate(byComputer, theme).text);
    }

    TEST(EventNarrationTests, NamesTheCellSomethingHappenedOn) {
        const flow::MatchEvent event{
            .kind = flow::MatchEventKind::ShipSunk,
            .coordinate = core::Coordinate{2, 4}
        };

        EXPECT_NE(narrate(event, defaultTheme()).text.find("C5"), std::string::npos);
    }

    TEST(EventNarrationTests, CopesWithAnEventThatHappenedNowhere) {
        const flow::MatchEvent event{.kind = flow::MatchEventKind::ShotMissed};

        EXPECT_FALSE(narrate(event, defaultTheme()).text.empty());
    }

    TEST(EventNarrationTests, ColoursALineFromTheTheme) {
        const flow::MatchEvent event{
            .kind = flow::MatchEventKind::ShipSunk,
            .coordinate = core::Coordinate{0, 0}
        };

        EXPECT_EQ(narrate(event, defaultTheme()).color, defaultTheme().sunk.fill);
    }

    TEST(NoticesTests, ShowsNothingWhenNothingWasSaid) {
        ContextFixture fixture;

        EXPECT_TRUE(noticesToShow(fixture.context()).empty());
    }

    TEST(NoticesTests, ShowsWhatWasSaid) {
        ContextFixture fixture;
        fixture.context().note("something happened");

        const std::vector<std::string> shown = noticesToShow(fixture.context());

        ASSERT_EQ(shown.size(), 1U);
        EXPECT_EQ(shown.front(), "something happened");
    }

    TEST(NoticesTests, ShowsOnlyTheNewestFew) {
        ContextFixture fixture;
        fixture.context().note("first");
        fixture.context().note("second");
        fixture.context().note("third");

        const std::vector<std::string> shown = noticesToShow(fixture.context());

        ASSERT_EQ(shown.size(), static_cast<std::size_t>(NOTICES_SHOWN));
        EXPECT_EQ(shown.front(), "second");
        EXPECT_EQ(shown.back(), "third");
    }

    TEST(BattleStateTests, AShortLogDoesNotScroll) {
        EXPECT_EQ(state::furthestLogScroll(0), 0);
        EXPECT_EQ(state::furthestLogScroll(state::LOG_VISIBLE_LINES), 0);
    }

    TEST(BattleStateTests, ALongLogScrollsByWhatDoesNotFit) {
        EXPECT_EQ(state::furthestLogScroll(state::LOG_VISIBLE_LINES + 5), 5);
    }

    TEST(BattleStateTests, ANegativeCountNeverScrollsBackwards) {
        EXPECT_EQ(state::furthestLogScroll(-3), 0);
    }
}  // namespace cpp_warships::head::common::render
