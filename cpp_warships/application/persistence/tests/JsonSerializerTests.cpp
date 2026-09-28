#include <application/core/Board.h>
#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/core/FleetComposition.h>
#include <application/core/MatchSettings.h>
#include <application/core/Segment.h>
#include <application/core/Ship.h>
#include <application/persistence/serializers/BoardJsonSerializer.h>
#include <application/persistence/serializers/MatchSettingsJsonSerializer.h>
#include <application/persistence/serializers/SegmentJsonSerializer.h>
#include <application/persistence/serializers/ShipJsonSerializer.h>
#include <gtest/gtest.h>
#include <serialization/exceptions/DeserializationException.h>

#include <map>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::persistence::serializers {
    namespace {
        /** @brief A ship serializer with its segment serializer wired up. */
        class WiredShipSerializer {
        public:
            WiredShipSerializer() {
                serializer.setChildrenSerializers(&segmentSerializer);
            }

            ShipJsonSerializer serializer;

        private:
            SegmentJsonSerializer segmentSerializer;
        };

        /** @brief A board serializer with the whole chain below it wired up. */
        class WiredBoardSerializer {
        public:
            WiredBoardSerializer() {
                shipSerializer.setChildrenSerializers(&segmentSerializer);
                serializer.setChildrenSerializers(&shipSerializer);
            }

            BoardJsonSerializer serializer;

        private:
            SegmentJsonSerializer segmentSerializer;
            ShipJsonSerializer shipSerializer;
        };
    }  // namespace

    TEST(SegmentJsonSerializerTests, NamesItsType) {
        SegmentJsonSerializer serializer;

        EXPECT_EQ(serializer.getType(), "Segment");
    }

    TEST(SegmentJsonSerializerTests, WritesBothHealthValues) {
        SegmentJsonSerializer serializer;
        core::Segment segment{3, 1};

        const nlohmann::json written = serializer.serialize(segment);

        EXPECT_EQ(written["maximumHealth"], 3);
        EXPECT_EQ(written["health"], 1);
    }

    TEST(SegmentJsonSerializerTests, ReadsBackWhatItWrote) {
        SegmentJsonSerializer serializer;
        core::Segment original{4, 2};

        const core::Segment restored = serializer.deserialize(serializer.serialize(original));

        EXPECT_EQ(restored.maximumHealth(), 4);
        EXPECT_EQ(restored.health(), 2);
    }

    TEST(SegmentJsonSerializerTests, RecognisesOnlyItsOwnShape) {
        SegmentJsonSerializer serializer;

        EXPECT_TRUE(serializer.isRelated({{"maximumHealth", 2}, {"health", 1}}));
        EXPECT_FALSE(serializer.isRelated({{"maximumHealth", 2}}));
        EXPECT_FALSE(serializer.isRelated(nlohmann::json::object()));
    }

    TEST(SegmentJsonSerializerTests, RefusesJsonThatIsNotASegment) {
        SegmentJsonSerializer serializer;

        EXPECT_THROW(
            (void)serializer.deserialize(nlohmann::json{{"health", 1}}),
            serialization::exceptions::DeserializationException
        );
    }

    TEST(ShipJsonSerializerTests, NamesItsType) {
        WiredShipSerializer wired;

        EXPECT_EQ(wired.serializer.getType(), "Ship");
    }

    TEST(ShipJsonSerializerTests, WritesOriginDirectionAndSegments) {
        WiredShipSerializer wired;
        core::Ship ship{{2, 5}, core::Direction::Vertical, 3};

        const nlohmann::json written = wired.serializer.serialize(ship);

        EXPECT_EQ(written["origin"]["x"], 2);
        EXPECT_EQ(written["origin"]["y"], 5);
        EXPECT_EQ(written["direction"], "vertical");
        EXPECT_EQ(written["segments"].size(), 3U);
    }

    TEST(ShipJsonSerializerTests, WritesAHorizontalShipAsHorizontal) {
        WiredShipSerializer wired;
        core::Ship ship{{0, 0}, core::Direction::Horizontal, 2};

        EXPECT_EQ(wired.serializer.serialize(ship)["direction"], "horizontal");
    }

    TEST(ShipJsonSerializerTests, ReadsBackWhatItWrote) {
        WiredShipSerializer wired;
        core::Ship original{{3, 4}, core::Direction::Vertical, 3, 2};
        original.damageSegment(1, 1);

        const core::Ship restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(restored.origin(), (core::Coordinate{3, 4}));
        EXPECT_EQ(restored.direction(), core::Direction::Vertical);
        EXPECT_EQ(restored.length(), 3);
        EXPECT_EQ(restored.segmentHealth(0), 2);
        EXPECT_EQ(restored.segmentHealth(1), 1);
    }

    TEST(ShipJsonSerializerTests, CarriesASunkShipBackAsSunk) {
        WiredShipSerializer wired;
        core::Ship original{{0, 0}, core::Direction::Horizontal, 2, 1};
        original.damageSegment(0, 1);
        original.damageSegment(1, 1);

        const core::Ship restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_TRUE(restored.isSunk());
    }

    TEST(ShipJsonSerializerTests, RecognisesOnlyItsOwnShape) {
        WiredShipSerializer wired;

        EXPECT_TRUE(wired.serializer.isRelated(
            {{"origin", {{"x", 0}, {"y", 0}}},
             {"direction", "horizontal"},
             {"segments", nlohmann::json::array()}}
        ));
        EXPECT_FALSE(wired.serializer.isRelated({{"origin", {{"x", 0}, {"y", 0}}}}));
    }

    TEST(ShipJsonSerializerTests, RefusesJsonThatIsNotAShip) {
        WiredShipSerializer wired;

        EXPECT_THROW(
            (void)wired.serializer.deserialize(nlohmann::json{{"direction", "horizontal"}}),
            serialization::exceptions::DeserializationException
        );
    }

    TEST(BoardJsonSerializerTests, NamesItsType) {
        WiredBoardSerializer wired;

        EXPECT_EQ(wired.serializer.getType(), "Board");
    }

    TEST(BoardJsonSerializerTests, WritesSizeShipsAndShots) {
        WiredBoardSerializer wired;
        core::Board board{8, 6};
        board.place({0, 0}, core::Direction::Horizontal, 2);
        board.attack({5, 5}, 1);

        const nlohmann::json written = wired.serializer.serialize(board);

        EXPECT_EQ(written["width"], 8);
        EXPECT_EQ(written["height"], 6);
        EXPECT_EQ(written["ships"].size(), 1U);
        EXPECT_EQ(written["attackedCells"].size(), 1U);
    }

    TEST(BoardJsonSerializerTests, ReadsBackWhatItWrote) {
        WiredBoardSerializer wired;
        core::Board original{10, 10};
        original.place({1, 1}, core::Direction::Horizontal, 3, 2);
        original.place({5, 5}, core::Direction::Vertical, 2, 2);
        original.attack({1, 1}, 1);
        original.attack({9, 9}, 1);

        const core::Board restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(restored.width(), 10);
        EXPECT_EQ(restored.height(), 10);
        EXPECT_EQ(restored.ships().size(), 2U);
        EXPECT_EQ(restored.attackedCells(), original.attackedCells());
    }

    TEST(BoardJsonSerializerTests, CarriesDamageBackWithTheBoard) {
        WiredBoardSerializer wired;
        core::Board original{10, 10};
        original.place({1, 1}, core::Direction::Horizontal, 2, 2);
        original.attack({1, 1}, 1);

        const core::Board restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(restored.stateAt({1, 1}, core::Visibility::Opponent), core::CellState::Damaged);
    }

    TEST(BoardJsonSerializerTests, CarriesAnEmptyBoardBack) {
        WiredBoardSerializer wired;
        core::Board original{4, 4};

        const core::Board restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(restored.width(), 4);
        EXPECT_FALSE(restored.hasShips());
        EXPECT_TRUE(restored.attackedCells().empty());
    }

    TEST(BoardJsonSerializerTests, RecognisesOnlyItsOwnShape) {
        WiredBoardSerializer wired;

        EXPECT_FALSE(wired.serializer.isRelated({{"width", 8}, {"height", 8}}));
    }

    TEST(BoardJsonSerializerTests, RefusesJsonThatIsNotABoard) {
        WiredBoardSerializer wired;

        EXPECT_THROW(
            (void)wired.serializer.deserialize(nlohmann::json{{"width", 8}}),
            serialization::exceptions::DeserializationException
        );
    }

    TEST(MatchSettingsJsonSerializerTests, NamesItsType) {
        MatchSettingsJsonSerializer serializer;

        EXPECT_EQ(serializer.getType(), "MatchSettings");
    }

    TEST(MatchSettingsJsonSerializerTests, WritesEveryRule) {
        MatchSettingsJsonSerializer serializer;
        core::MatchSettings settings{
            12,
            core::FleetComposition{std::map<int, int>{{1, 2}, {3, 1}}},
            2,
            3
        };

        const nlohmann::json written = serializer.serialize(settings);

        EXPECT_EQ(written["boardSize"], 12);
        EXPECT_EQ(written["baseDamage"], 2);
        EXPECT_EQ(written["segmentHealth"], 3);
        EXPECT_EQ(written["fleet"].size(), 2U);
    }

    TEST(MatchSettingsJsonSerializerTests, ReadsBackWhatItWrote) {
        MatchSettingsJsonSerializer serializer;
        core::MatchSettings original = core::MatchSettings::forBoardSize(10);

        const core::MatchSettings restored = serializer.deserialize(serializer.serialize(original));

        EXPECT_EQ(restored.boardSize(), original.boardSize());
        EXPECT_EQ(restored.baseDamage(), original.baseDamage());
        EXPECT_EQ(restored.segmentHealth(), original.segmentHealth());
        EXPECT_EQ(restored.fleet().countsByLength(), original.fleet().countsByLength());
    }

    TEST(MatchSettingsJsonSerializerTests, CarriesAnEmptyFleetBack) {
        MatchSettingsJsonSerializer serializer;
        core::MatchSettings original{8, core::FleetComposition{}};

        const core::MatchSettings restored = serializer.deserialize(serializer.serialize(original));

        EXPECT_TRUE(restored.fleet().isEmpty());
    }

    TEST(MatchSettingsJsonSerializerTests, RefusesJsonThatIsNotSettings) {
        MatchSettingsJsonSerializer serializer;

        EXPECT_FALSE(serializer.isRelated({{"boardSize", 10}}));
        EXPECT_THROW(
            (void)serializer.deserialize(nlohmann::json{{"boardSize", 10}}),
            serialization::exceptions::DeserializationException
        );
    }
}  // namespace cpp_warships::persistence::serializers
