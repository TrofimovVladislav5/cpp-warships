#include <gtest/gtest.h>
#include <platform/OSBrancher.h>

#include <chrono>
#include <ctime>
#include <optional>
#include <string>

namespace cpp_warships::platform {
    namespace {
        /** @brief A name no environment is expected to have set. */
        constexpr const char* UNSET_NAME = "CPP_WARSHIPS_NAME_NOTHING_SETS";
    }  // namespace

    TEST(OSBrancherTests, ReadsAValueTheEnvironmentHasSet) {
        const std::optional<std::string> path = environmentValue("PATH");

        ASSERT_TRUE(path.has_value());
        EXPECT_FALSE(path->empty());
    }

    TEST(OSBrancherTests, ReadsNothingForANameTheEnvironmentHasNotSet) {
        EXPECT_EQ(environmentValue(UNSET_NAME), std::nullopt);
    }

    TEST(OSBrancherTests, FindsSomewhereToCallHome) {
        const std::optional<std::string> home = homeDirectory();

        ASSERT_TRUE(home.has_value());
        EXPECT_FALSE(home->empty());
    }

    TEST(OSBrancherTests, BreaksAMomentIntoLocalCalendarFields) {
        const std::tm broken = localTimeOf(std::time_t{0});

        EXPECT_GE(broken.tm_year, 69);
        EXPECT_GE(broken.tm_mon, 0);
        EXPECT_LE(broken.tm_mon, 11);
        EXPECT_GE(broken.tm_mday, 1);
        EXPECT_LE(broken.tm_mday, 31);
    }

    TEST(OSBrancherTests, BreaksTheMomentItIsGivenRatherThanAnyOther) {
        const std::time_t now =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        const std::tm earlier = localTimeOf(now - 60 * 60 * 24);
        const std::tm later = localTimeOf(now);

        EXPECT_NE(earlier.tm_yday, later.tm_yday);
    }

    TEST(OSBrancherTests, AnswersWhetherOutputIsATerminalWithoutComplaint) {
        EXPECT_NO_THROW((void)isOutputTerminal());
    }

    TEST(OSBrancherTests, PreparesTheConsoleWithoutComplaint) {
        EXPECT_NO_THROW(prepareConsoleForUnicode());
    }
}  // namespace cpp_warships::platform
