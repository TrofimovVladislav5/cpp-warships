#include <gtest/gtest.h>
#include <utilities/StringHelper.h>

#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

TEST(StringHelperTests, SplitsOnTheDelimiter) {
    EXPECT_EQ(StringHelper::split("a,b,c", ','), (std::vector<std::string>{"a", "b", "c"}));
}

TEST(StringHelperTests, SplittingSomethingWithoutTheDelimiterGivesItBackWhole) {
    EXPECT_EQ(StringHelper::split("abc", ','), (std::vector<std::string>{"abc"}));
}

TEST(StringHelperTests, SplittingNothingGivesNothing) {
    EXPECT_TRUE(StringHelper::split("", ',').empty());
}

TEST(StringHelperTests, KeepsAnEmptyPieceBetweenTwoDelimiters) {
    EXPECT_EQ(StringHelper::split("a,,b", ','), (std::vector<std::string>{"a", "", "b"}));
}

TEST(StringHelperTests, DropsATrailingEmptyPiece) {
    EXPECT_EQ(StringHelper::split("a,b,", ','), (std::vector<std::string>{"a", "b"}));
}

TEST(StringHelperTests, KeepsALeadingEmptyPiece) {
    EXPECT_EQ(StringHelper::split(",a", ','), (std::vector<std::string>{"", "a"}));
}

TEST(StringHelperTests, LowersEveryLetter) {
    EXPECT_EQ(StringHelper::toLower("ABC"), "abc");
    EXPECT_EQ(StringHelper::toLower("MiXeD"), "mixed");
}

TEST(StringHelperTests, LeavesWhatIsNotALetterAlone) {
    EXPECT_EQ(StringHelper::toLower("a1-B2"), "a1-b2");
    EXPECT_EQ(StringHelper::toLower(""), "");
}

TEST(StringHelperTests, TrimsSpacesFromBothEnds) {
    EXPECT_EQ(StringHelper::trim("  abc  "), "abc");
    EXPECT_EQ(StringHelper::trim("abc  "), "abc");
    EXPECT_EQ(StringHelper::trim("  abc"), "abc");
}

TEST(StringHelperTests, LeavesSpacesInTheMiddleAlone) {
    EXPECT_EQ(StringHelper::trim("  a b c  "), "a b c");
}

TEST(StringHelperTests, TrimmingNothingOrOnlySpacesGivesNothing) {
    EXPECT_EQ(StringHelper::trim(""), "");
    EXPECT_EQ(StringHelper::trim("   "), "");
}

TEST(StringHelperTests, TrimmingSomethingAlreadyTrimmedChangesNothing) {
    EXPECT_EQ(StringHelper::trim("abc"), "abc");
}

TEST(StringHelperTests, BuildsACoordinatePatternForASupportedFieldSize) {
    const std::string pattern = StringHelper::patternCoordinate(10);

    EXPECT_FALSE(pattern.empty());
    EXPECT_TRUE(std::regex_match("3,4", std::regex{pattern}));
    EXPECT_FALSE(std::regex_match("abc", std::regex{pattern}));
}

TEST(StringHelperTests, ACoordinatePatternForALargeFieldAcceptsTwoDigits) {
    const std::regex pattern{StringHelper::patternCoordinate(20)};

    EXPECT_TRUE(std::regex_match("15,19", pattern));
    EXPECT_TRUE(std::regex_match("0,0", pattern));
}

TEST(StringHelperTests, RefusesAFieldSizeItCannotDescribe) {
    EXPECT_THROW((void)StringHelper::patternCoordinate(9), std::invalid_argument);
    EXPECT_THROW((void)StringHelper::patternCoordinate(27), std::invalid_argument);
    EXPECT_THROW((void)StringHelper::patternCoordinate(0), std::invalid_argument);
}
