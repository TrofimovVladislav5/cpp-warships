#include <application/core/Coordinate.h>
#include <application/model/events/EventHandler.h>
#include <application/model/events/EventQueue.h>
#include <application/model/events/EventRouter.h>
#include <application/model/events/EventScope.h>
#include <application/model/events/GameEvent.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace cpp_warships::model::events {
    namespace {
        /** @brief A handler that claims one kind of event and counts what it took. */
        template <typename TEvent>
        class CountingHandler final : public EventHandler {
        public:
            [[nodiscard]] bool isHandled(const GameEvent& event) const override {
                return isKind<TEvent>(event);
            }

            void handleEvent(const GameEvent&) override {
                ++handledCount;
            }

            int handledCount = 0;
        };

        /** @brief A handler that claims nothing at all. */
        class DeafHandler final : public EventHandler {
        public:
            [[nodiscard]] bool isHandled(const GameEvent&) const override {
                return false;
            }

            void handleEvent(const GameEvent&) override {
                ++handledCount;
            }

            int handledCount = 0;
        };
    }  // namespace

    TEST(GameEventTests, RecognisesTheKindItHolds) {
        const GameEvent event = SessionQuitRequested{};

        EXPECT_TRUE(isKind<SessionQuitRequested>(event));
        EXPECT_FALSE(isKind<MatchResumeRequested>(event));
    }

    TEST(GameEventTests, CarriesTheBoardSizeOfANewMatch) {
        const GameEvent event = MatchStartRequested{.boardSize = 14};

        ASSERT_TRUE(isKind<MatchStartRequested>(event));
        EXPECT_EQ(std::get<MatchStartRequested>(event).boardSize, 14);
    }

    TEST(GameEventTests, ANewMatchDefaultsToATenByTenBoard) {
        EXPECT_EQ(MatchStartRequested{}.boardSize, 10);
    }

    TEST(GameEventTests, CarriesTheNameOfASaveToLoad) {
        const GameEvent event = MatchLoadRequested{.name = "20260928-120000"};

        EXPECT_EQ(std::get<MatchLoadRequested>(event).name, "20260928-120000");
    }

    TEST(GameEventTests, CarriesWhereAShipGoes) {
        const GameEvent event = ShipPlacementRequested{
            .origin = core::Coordinate{2, 3},
            .direction = core::Direction::Vertical,
            .length = 4
        };

        const auto& placement = std::get<ShipPlacementRequested>(event);
        EXPECT_EQ(placement.origin, (core::Coordinate{2, 3}));
        EXPECT_EQ(placement.direction, core::Direction::Vertical);
        EXPECT_EQ(placement.length, 4);
    }

    TEST(GameEventTests, ASkillMayBeAskedForWithoutAnAim) {
        const GameEvent aimless = SkillUseRequested{};
        const GameEvent aimed = SkillUseRequested{.aim = core::Coordinate{1, 1}};

        EXPECT_FALSE(std::get<SkillUseRequested>(aimless).aim.has_value());
        EXPECT_EQ(std::get<SkillUseRequested>(aimed).aim, (core::Coordinate{1, 1}));
    }

    TEST(EventQueueTests, StartsEmpty) {
        const EventQueue queue;

        EXPECT_TRUE(queue.isEmpty());
    }

    TEST(EventQueueTests, DrainsInTheOrderEventsArrived) {
        EventQueue queue;
        queue.push(MatchStartRequested{.boardSize = 8});
        queue.push(SessionQuitRequested{});

        const std::vector<GameEvent> drained = queue.drain();

        ASSERT_EQ(drained.size(), 2U);
        EXPECT_TRUE(isKind<MatchStartRequested>(drained[0]));
        EXPECT_TRUE(isKind<SessionQuitRequested>(drained[1]));
    }

    TEST(EventQueueTests, DrainingLeavesTheQueueEmpty) {
        EventQueue queue;
        queue.push(SessionQuitRequested{});

        (void)queue.drain();

        EXPECT_TRUE(queue.isEmpty());
        EXPECT_TRUE(queue.drain().empty());
    }

    TEST(EventQueueTests, PushingMakesItUnempty) {
        EventQueue queue;

        queue.push(MenuReturnRequested{});

        EXPECT_FALSE(queue.isEmpty());
    }

    TEST(EventRouterTests, StartsInTheMenu) {
        const EventRouter router;

        EXPECT_EQ(router.currentScope(), EventScope::Menu);
    }

    TEST(EventRouterTests, RemembersTheScopeItWasSentTo) {
        EventRouter router;

        router.enterScope(EventScope::Battle);

        EXPECT_EQ(router.currentScope(), EventScope::Battle);
    }

    TEST(EventRouterTests, AnEventNobodyWantsGoesNowhere) {
        EventRouter router;

        EXPECT_FALSE(router.dispatch(SessionQuitRequested{}));
    }

    TEST(EventRouterTests, HandsAnEventToAHandlerInScope) {
        EventRouter router;
        const auto handler = std::make_shared<CountingHandler<SessionQuitRequested>>();
        router.subscribe(EventScope::Menu, handler);

        EXPECT_TRUE(router.dispatch(SessionQuitRequested{}));

        EXPECT_EQ(handler->handledCount, 1);
    }

    TEST(EventRouterTests, WithholdsAnEventFromAHandlerOutOfScope) {
        EventRouter router;
        const auto handler = std::make_shared<CountingHandler<SessionQuitRequested>>();
        router.subscribe(EventScope::Battle, handler);

        EXPECT_FALSE(router.dispatch(SessionQuitRequested{}));

        EXPECT_EQ(handler->handledCount, 0);
    }

    TEST(EventRouterTests, AHandlerListeningAlwaysHearsEveryScope) {
        EventRouter router;
        const auto handler = std::make_shared<CountingHandler<SessionQuitRequested>>();
        router.subscribe(EventScope::Always, handler);

        router.enterScope(EventScope::Placement);
        EXPECT_TRUE(router.dispatch(SessionQuitRequested{}));

        router.enterScope(EventScope::Battle);
        EXPECT_TRUE(router.dispatch(SessionQuitRequested{}));

        EXPECT_EQ(handler->handledCount, 2);
    }

    TEST(EventRouterTests, WithholdsAnEventAHandlerDoesNotClaim) {
        EventRouter router;
        const auto handler = std::make_shared<CountingHandler<SessionQuitRequested>>();
        router.subscribe(EventScope::Menu, handler);

        EXPECT_FALSE(router.dispatch(MenuReturnRequested{}));

        EXPECT_EQ(handler->handledCount, 0);
    }

    TEST(EventRouterTests, OnlyTheFirstClaimantTakesAnEvent) {
        EventRouter router;
        const auto first = std::make_shared<CountingHandler<SessionQuitRequested>>();
        const auto second = std::make_shared<CountingHandler<SessionQuitRequested>>();
        router.subscribe(EventScope::Menu, first);
        router.subscribe(EventScope::Menu, second);

        router.dispatch(SessionQuitRequested{});

        EXPECT_EQ(first->handledCount, 1);
        EXPECT_EQ(second->handledCount, 0);
    }

    TEST(EventRouterTests, PassesOverAHandlerThatClaimsNothing) {
        EventRouter router;
        const auto deaf = std::make_shared<DeafHandler>();
        const auto listening = std::make_shared<CountingHandler<SessionQuitRequested>>();
        router.subscribe(EventScope::Menu, deaf);
        router.subscribe(EventScope::Menu, listening);

        EXPECT_TRUE(router.dispatch(SessionQuitRequested{}));

        EXPECT_EQ(deaf->handledCount, 0);
        EXPECT_EQ(listening->handledCount, 1);
    }

    TEST(EventRouterTests, FollowsThePlayerFromOneScopeToAnother) {
        EventRouter router;
        const auto menuHandler = std::make_shared<CountingHandler<SessionQuitRequested>>();
        const auto battleHandler = std::make_shared<CountingHandler<SessionQuitRequested>>();
        router.subscribe(EventScope::Menu, menuHandler);
        router.subscribe(EventScope::Battle, battleHandler);

        router.dispatch(SessionQuitRequested{});
        router.enterScope(EventScope::Battle);
        router.dispatch(SessionQuitRequested{});

        EXPECT_EQ(menuHandler->handledCount, 1);
        EXPECT_EQ(battleHandler->handledCount, 1);
    }
}  // namespace cpp_warships::model::events
