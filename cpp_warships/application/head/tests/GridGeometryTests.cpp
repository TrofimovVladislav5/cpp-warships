#include <application/core/Coordinate.h>
#include <application/head/common/input/GridGeometry.h>
#include <gtest/gtest.h>

#include <optional>

namespace cpp_warships::head::common::input {
    namespace {
        constexpr int COLUMN_PITCH = 2;
        constexpr int ROW_PITCH = 1;

        /** @brief Remembers an 8x8 board drawn at (10, 5), two columns per cell. */
        void rememberEnemyBoard(GridGeometry& geometry) {
            geometry.rememberBoard(
                ScreenRegion::EnemyWaters,
                10,
                5,
                16,
                8,
                8,
                8,
                COLUMN_PITCH,
                ROW_PITCH
            );
        }
    }  // namespace

    TEST(GridGeometryTests, KnowsNothingUntilSomethingIsDrawn) {
        const GridGeometry geometry;

        EXPECT_EQ(geometry.regionAt(0, 0), ScreenRegion::Elsewhere);
        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 0, 0), std::nullopt);
    }

    TEST(GridGeometryTests, PlacesAPointerInTheRegionItIsOver) {
        GridGeometry geometry;
        rememberEnemyBoard(geometry);

        EXPECT_EQ(geometry.regionAt(10, 5), ScreenRegion::EnemyWaters);
        EXPECT_EQ(geometry.regionAt(25, 12), ScreenRegion::EnemyWaters);
    }

    TEST(GridGeometryTests, PlacesAPointerOutsideEveryRegionElsewhere) {
        GridGeometry geometry;
        rememberEnemyBoard(geometry);

        EXPECT_EQ(geometry.regionAt(9, 5), ScreenRegion::Elsewhere);
        EXPECT_EQ(geometry.regionAt(26, 5), ScreenRegion::Elsewhere);
        EXPECT_EQ(geometry.regionAt(10, 4), ScreenRegion::Elsewhere);
        EXPECT_EQ(geometry.regionAt(10, 13), ScreenRegion::Elsewhere);
    }

    TEST(GridGeometryTests, TurnsAPointerIntoTheCellUnderIt) {
        GridGeometry geometry;
        rememberEnemyBoard(geometry);

        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 10, 5), (core::Coordinate{0, 0}));
        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 12, 5), (core::Coordinate{1, 0}));
        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 10, 6), (core::Coordinate{0, 1}));
    }

    TEST(GridGeometryTests, EveryColumnOfACellAnswersAsThatCell) {
        GridGeometry geometry;
        rememberEnemyBoard(geometry);

        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 12, 5), (core::Coordinate{1, 0}));
        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 13, 5), (core::Coordinate{1, 0}));
    }

    TEST(GridGeometryTests, APointerOutsideThePatchIsOverNoCell) {
        GridGeometry geometry;
        rememberEnemyBoard(geometry);

        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 9, 5), std::nullopt);
        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 10, 20), std::nullopt);
    }

    TEST(GridGeometryTests, APatchWiderThanItsBoardStopsAtTheLastCell) {
        GridGeometry geometry;
        geometry.rememberBoard(ScreenRegion::OwnWaters, 0, 0, 20, 4, 4, 4, 2, 1);

        EXPECT_EQ(geometry.cellAt(ScreenRegion::OwnWaters, 6, 0), (core::Coordinate{3, 0}));
        EXPECT_EQ(geometry.cellAt(ScreenRegion::OwnWaters, 8, 0), std::nullopt);
    }

    TEST(GridGeometryTests, TellsTheTwoBoardsApart) {
        GridGeometry geometry;
        geometry.rememberBoard(ScreenRegion::OwnWaters, 0, 0, 8, 4, 4, 4, 2, 1);
        geometry.rememberBoard(ScreenRegion::EnemyWaters, 20, 0, 8, 4, 4, 4, 2, 1);

        EXPECT_EQ(geometry.regionAt(0, 0), ScreenRegion::OwnWaters);
        EXPECT_EQ(geometry.regionAt(20, 0), ScreenRegion::EnemyWaters);
        EXPECT_EQ(geometry.cellAt(ScreenRegion::OwnWaters, 2, 0), (core::Coordinate{1, 0}));
        EXPECT_EQ(geometry.cellAt(ScreenRegion::EnemyWaters, 22, 0), (core::Coordinate{1, 0}));
    }

    TEST(GridGeometryTests, RemembersWhereTheLogWasDrawn) {
        GridGeometry geometry;
        geometry.rememberLog(40, 2, 30, 12);

        EXPECT_EQ(geometry.regionAt(45, 6), ScreenRegion::Log);
        EXPECT_EQ(geometry.regionAt(39, 6), ScreenRegion::Elsewhere);
    }

    TEST(GridGeometryTests, TheLogHoldsNoCells) {
        GridGeometry geometry;
        geometry.rememberLog(40, 2, 30, 12);

        EXPECT_EQ(geometry.cellAt(ScreenRegion::Log, 45, 6), std::nullopt);
    }

    TEST(GridGeometryTests, ClearingForgetsEverythingDrawn) {
        GridGeometry geometry;
        rememberEnemyBoard(geometry);
        geometry.rememberLog(40, 2, 30, 12);

        geometry.clear();

        EXPECT_EQ(geometry.regionAt(10, 5), ScreenRegion::Elsewhere);
        EXPECT_EQ(geometry.regionAt(45, 6), ScreenRegion::Elsewhere);
    }

    TEST(GridGeometryTests, RememberingAgainReplacesWhereARegionWas) {
        GridGeometry geometry;
        rememberEnemyBoard(geometry);

        geometry.rememberBoard(ScreenRegion::EnemyWaters, 0, 0, 8, 4, 4, 4, 2, 1);

        EXPECT_EQ(geometry.regionAt(10, 5), ScreenRegion::Elsewhere);
        EXPECT_EQ(geometry.regionAt(0, 0), ScreenRegion::EnemyWaters);
    }

    TEST(GridGeometryTests, NothingIsEverOverTheElsewhereRegion) {
        GridGeometry geometry;
        rememberEnemyBoard(geometry);

        EXPECT_EQ(geometry.cellAt(ScreenRegion::Elsewhere, 10, 5), std::nullopt);
    }

    TEST(GridGeometryTests, APitchOfNothingYieldsNoCell) {
        GridGeometry geometry;
        geometry.rememberBoard(ScreenRegion::OwnWaters, 0, 0, 8, 4, 4, 4, 0, 1);

        EXPECT_EQ(geometry.cellAt(ScreenRegion::OwnWaters, 2, 0), std::nullopt);
    }
}  // namespace cpp_warships::head::common::input
