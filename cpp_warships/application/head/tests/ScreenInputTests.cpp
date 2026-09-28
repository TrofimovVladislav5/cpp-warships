#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/head/common/PresentationContext.h>
#include <application/head/common/ScreenKind.h>
#include <application/head/common/input/BattleInput.h>
#include <application/head/common/input/EventBus.h>
#include <application/head/common/input/Keystroke.h>
#include <application/head/common/input/MenuInput.h>
#include <application/head/common/input/PlacementInput.h>
#include <application/head/common/input/SaveBrowserInput.h>
#include <application/head/common/input/SaveNamingInput.h>
#include <application/head/common/input/ScreenInput.h>
#include <application/model/ApplicationContext.h>
#include <application/model/WarshipsGame.h>
#include <application/model/events/GameEvent.h>
#include <application/persistence/MemorySaveStorage.h>
#include <application/persistence/SaveArchive.h>
#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <variant>

namespace cpp_warships::head::common::input {
    namespace {
        constexpr int BOARD_SIZE = 10;

        /** @brief A presentation context over a real game, all kept alive together. */
        class PresentationFixture {
        public:
            PresentationFixture()
                : archive_(storage_)
                , game_(randomEngine_, archive_)
                , application_(game_)
                , context_(application_) {}

            PresentationContext& context() noexcept {
                return context_;
            }

            model::WarshipsGame& game() noexcept {
                return game_;
            }

        private:
            flow::RandomEngine randomEngine_{17U};
            persistence::MemorySaveStorage storage_;
            persistence::SaveArchive archive_;
            model::WarshipsGame game_;
            model::ApplicationContext application_;
            PresentationContext context_;
        };

        Keystroke typed(const std::string& character) {
            return Keystroke{.key = Key::Character, .character = character};
        }

        Keystroke pressed(Key key) {
            return Keystroke{.key = key};
        }
    }  // namespace

    TEST(MenuInputTests, EnterAsksForAMatchOnTheChosenBoardSize) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};
        fixture.context().state().menu.selectedBoardSize = 14;

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Enter));

        ASSERT_TRUE(event.has_value());
        ASSERT_TRUE(model::events::isKind<model::events::MatchStartRequested>(*event));
        EXPECT_EQ(std::get<model::events::MatchStartRequested>(*event).boardSize, 14);
    }

    TEST(MenuInputTests, ArrowsResizeTheBoardWithoutAskingTheGameForAnything) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};
        fixture.context().state().menu.selectedBoardSize = 10;

        EXPECT_FALSE(input.interpret(pressed(Key::ArrowRight)).has_value());
        EXPECT_EQ(fixture.context().state().menu.selectedBoardSize, 12);

        EXPECT_FALSE(input.interpret(pressed(Key::ArrowLeft)).has_value());
        EXPECT_EQ(fixture.context().state().menu.selectedBoardSize, 10);
    }

    TEST(MenuInputTests, TheBoardSizeStopsAtItsLimits) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};

        for (int press = 0; press < 20; ++press) {
            (void)input.interpret(pressed(Key::ArrowLeft));
        }
        EXPECT_EQ(fixture.context().state().menu.selectedBoardSize, 8);

        for (int press = 0; press < 20; ++press) {
            (void)input.interpret(pressed(Key::ArrowRight));
        }
        EXPECT_EQ(fixture.context().state().menu.selectedBoardSize, 20);
    }

    TEST(MenuInputTests, TypingTCyclesTheTheme) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};
        const std::string before = fixture.context().theme().name;

        EXPECT_FALSE(input.interpret(typed("t")).has_value());

        EXPECT_NE(fixture.context().theme().name, before);
    }

    TEST(MenuInputTests, TypingRAsksToResume) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(typed("r"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::MatchResumeRequested>(*event));
    }

    TEST(MenuInputTests, TypingLOpensTheSaves) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(typed("l"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::SaveBrowserRequested>(*event));
    }

    TEST(MenuInputTests, TypingSAsksWhatToCallTheMatch) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(typed("s"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::SaveNamingRequested>(*event));
    }

    TEST(MenuInputTests, TypingQAsksToStop) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(typed("q"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::SessionQuitRequested>(*event));
    }

    TEST(MenuInputTests, AKeyBoundToNothingAsksForNothing) {
        PresentationFixture fixture;
        MenuInput input{fixture.context()};

        EXPECT_FALSE(input.interpret(typed("z")).has_value());
        EXPECT_FALSE(input.interpret(pressed(Key::Tab)).has_value());
    }

    TEST(PlacementInputTests, ArrowsMoveTheCursorWithoutAskingForAnything) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};
        fixture.context().state().placement.cursor = core::Coordinate{0, 0};

        EXPECT_FALSE(input.interpret(pressed(Key::ArrowRight)).has_value());

        EXPECT_EQ(fixture.context().state().placement.cursor, (core::Coordinate{1, 0}));
    }

    TEST(PlacementInputTests, TheCursorStaysOnTheBoard) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};
        fixture.context().state().placement.cursor = core::Coordinate{0, 0};

        (void)input.interpret(pressed(Key::ArrowLeft));
        (void)input.interpret(pressed(Key::ArrowUp));

        EXPECT_EQ(fixture.context().state().placement.cursor, (core::Coordinate{0, 0}));
    }

    TEST(PlacementInputTests, TypingRTurnsTheShipInHand) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};
        fixture.context().state().placement.direction = core::Direction::Horizontal;

        EXPECT_FALSE(input.interpret(typed("r")).has_value());
        EXPECT_EQ(fixture.context().state().placement.direction, core::Direction::Vertical);

        EXPECT_FALSE(input.interpret(typed("r")).has_value());
        EXPECT_EQ(fixture.context().state().placement.direction, core::Direction::Horizontal);
    }

    TEST(PlacementInputTests, TabPicksAnotherShipLength) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};
        const int before = fixture.context().state().placement.preferredShipLength;

        EXPECT_FALSE(input.interpret(pressed(Key::Tab)).has_value());

        EXPECT_NE(fixture.context().state().placement.preferredShipLength, before);
    }

    TEST(PlacementInputTests, TypingBOpensFire) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(typed("b"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::BattleBeginRequested>(*event));
    }

    TEST(PlacementInputTests, EnterAsksForTheShipToBeLaid) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Enter));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::ShipPlacementRequested>(*event));
    }

    TEST(PlacementInputTests, BackspaceAsksForAShipToBeTakenBack) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event =
            input.interpret(pressed(Key::Backspace));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::ShipRemovalRequested>(*event));
    }

    TEST(PlacementInputTests, TypingFAsksForTheFleetToBeLaidOut) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(typed("f"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::FleetShuffleRequested>(*event));
    }

    TEST(PlacementInputTests, EscapeStepsBackToTheMenu) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        PlacementInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Escape));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::MenuReturnRequested>(*event));
    }

    TEST(BattleInputTests, EnterFiresAtWhereThePlayerIsAiming) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        fixture.game().play().beginBattle();
        BattleInput input{fixture.context()};
        fixture.context().state().battle.target = core::Coordinate{3, 4};

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Enter));

        ASSERT_TRUE(event.has_value());
        ASSERT_TRUE(model::events::isKind<model::events::ShotRequested>(*event));
        EXPECT_EQ(std::get<model::events::ShotRequested>(*event).target, (core::Coordinate{3, 4}));
    }

    TEST(BattleInputTests, ArrowsMoveTheAimWithoutFiring) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        fixture.game().play().beginBattle();
        BattleInput input{fixture.context()};
        fixture.context().state().battle.target = core::Coordinate{0, 0};

        EXPECT_FALSE(input.interpret(pressed(Key::ArrowDown)).has_value());

        EXPECT_EQ(fixture.context().state().battle.target, (core::Coordinate{0, 1}));
    }

    TEST(BattleInputTests, TypingKSpendsASkill) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        fixture.game().play().beginBattle();
        BattleInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(typed("k"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::SkillUseRequested>(*event));
    }

    TEST(BattleInputTests, EscapeStepsBackToTheMenu) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        fixture.game().play().shuffleFleet();
        fixture.game().play().beginBattle();
        BattleInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Escape));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::MenuReturnRequested>(*event));
    }

    TEST(SaveBrowserInputTests, EnterWithNothingSavedAsksForNothing) {
        PresentationFixture fixture;
        SaveBrowserInput input{fixture.context()};

        EXPECT_FALSE(input.interpret(pressed(Key::Enter)).has_value());
    }

    TEST(SaveBrowserInputTests, EnterLoadsTheSaveBeingLookedAt) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        (void)fixture.game().saves().saveMatch("my game");
        SaveBrowserInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Enter));

        ASSERT_TRUE(event.has_value());
        ASSERT_TRUE(model::events::isKind<model::events::MatchLoadRequested>(*event));
        EXPECT_EQ(
            std::get<model::events::MatchLoadRequested>(*event).name,
            fixture.game().saves().savedMatches().front().id
        );
    }

    TEST(SaveBrowserInputTests, TypingDThrowsTheSaveBeingLookedAtAway) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        (void)fixture.game().saves().saveMatch("my game");
        SaveBrowserInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(typed("d"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::SaveDeleteRequested>(*event));
    }

    TEST(SaveBrowserInputTests, ArrowsMoveThroughTheListWithoutLoadingAnything) {
        PresentationFixture fixture;
        fixture.game().play().startNewMatch(BOARD_SIZE);
        (void)fixture.game().saves().saveMatch("my game");
        SaveBrowserInput input{fixture.context()};

        EXPECT_FALSE(input.interpret(pressed(Key::ArrowDown)).has_value());

        EXPECT_EQ(fixture.context().state().saves.selectedIndex, 0);
    }

    TEST(SaveBrowserInputTests, EscapeStepsBackToTheMenu) {
        PresentationFixture fixture;
        SaveBrowserInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Escape));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::MenuReturnRequested>(*event));
    }

    TEST(SaveNamingInputTests, TypingBuildsUpTheName) {
        PresentationFixture fixture;
        SaveNamingInput input{fixture.context()};

        EXPECT_FALSE(input.interpret(typed("a")).has_value());
        EXPECT_FALSE(input.interpret(typed("b")).has_value());

        EXPECT_EQ(fixture.context().state().naming.typedName, "ab");
    }

    TEST(SaveNamingInputTests, BackspaceRubsTheLastLetterOut) {
        PresentationFixture fixture;
        SaveNamingInput input{fixture.context()};
        fixture.context().state().naming.typedName = "abc";

        (void)input.interpret(pressed(Key::Backspace));

        EXPECT_EQ(fixture.context().state().naming.typedName, "ab");
    }

    TEST(SaveNamingInputTests, BackspaceOnAnEmptyNameIsHarmless) {
        PresentationFixture fixture;
        SaveNamingInput input{fixture.context()};

        EXPECT_NO_THROW((void)input.interpret(pressed(Key::Backspace)));

        EXPECT_TRUE(fixture.context().state().naming.typedName.empty());
    }

    TEST(SaveNamingInputTests, AnEmptyNameCannotBeConfirmed) {
        PresentationFixture fixture;
        SaveNamingInput input{fixture.context()};

        EXPECT_FALSE(input.interpret(pressed(Key::Enter)).has_value());
    }

    TEST(SaveNamingInputTests, EnterPutsTheMatchAwayUnderTheNameTyped) {
        PresentationFixture fixture;
        SaveNamingInput input{fixture.context()};
        fixture.context().state().naming.typedName = "my game";

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Enter));

        ASSERT_TRUE(event.has_value());
        ASSERT_TRUE(model::events::isKind<model::events::MatchSaveAndQuitRequested>(*event));
        EXPECT_EQ(std::get<model::events::MatchSaveAndQuitRequested>(*event).name, "my game");
    }

    TEST(SaveNamingInputTests, ANameStopsGrowingAtItsLimit) {
        PresentationFixture fixture;
        SaveNamingInput input{fixture.context()};

        for (int press = 0; press < 60; ++press) {
            (void)input.interpret(typed("x"));
        }

        EXPECT_EQ(fixture.context().state().naming.typedName.size(), 40U);
    }

    TEST(SaveNamingInputTests, EscapeStepsBackToTheMenu) {
        PresentationFixture fixture;
        SaveNamingInput input{fixture.context()};

        const std::optional<model::events::GameEvent> event = input.interpret(pressed(Key::Escape));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::MenuReturnRequested>(*event));
    }

    TEST(EventBusTests, AScreenNobodyReadsAsksForNothing) {
        EventBus bus;

        EXPECT_FALSE(bus.interpret(ScreenKind::Menu, pressed(Key::Enter)).has_value());
    }

    TEST(EventBusTests, ReadsAStrokeThroughTheScreenItBelongsTo) {
        PresentationFixture fixture;
        EventBus bus;
        bus.readScreenWith(ScreenKind::Menu, std::make_unique<MenuInput>(fixture.context()));

        const std::optional<model::events::GameEvent> event =
            bus.interpret(ScreenKind::Menu, typed("q"));

        ASSERT_TRUE(event.has_value());
        EXPECT_TRUE(model::events::isKind<model::events::SessionQuitRequested>(*event));
    }

    TEST(EventBusTests, AStrokeMeansNothingOnAScreenThatIsNotShowing) {
        PresentationFixture fixture;
        EventBus bus;
        bus.readScreenWith(ScreenKind::Menu, std::make_unique<MenuInput>(fixture.context()));

        EXPECT_FALSE(bus.interpret(ScreenKind::Battle, typed("q")).has_value());
    }

    TEST(EventBusTests, PuttingAScreenInChargeAgainReplacesItsReader) {
        PresentationFixture fixture;
        EventBus bus;
        bus.readScreenWith(ScreenKind::Menu, std::make_unique<MenuInput>(fixture.context()));
        bus.readScreenWith(ScreenKind::Menu, std::make_unique<SaveNamingInput>(fixture.context()));

        EXPECT_FALSE(bus.interpret(ScreenKind::Menu, typed("q")).has_value());
        EXPECT_EQ(fixture.context().state().naming.typedName, "q");
    }
}  // namespace cpp_warships::head::common::input
