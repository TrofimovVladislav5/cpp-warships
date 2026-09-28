#include <application/head/common/Color.h>
#include <application/head/common/Theme.h>
#include <application/head/common/ThemeSelection.h>
#include <gtest/gtest.h>

#include <string>
#include <unordered_set>
#include <vector>

namespace cpp_warships::head::common {
    TEST(ColorTests, DefaultsToBlack) {
        const Color color;

        EXPECT_EQ(color.red, 0);
        EXPECT_EQ(color.green, 0);
        EXPECT_EQ(color.blue, 0);
    }

    TEST(ColorTests, ComparesChannelByChannel) {
        EXPECT_EQ((Color{1, 2, 3}), (Color{1, 2, 3}));
        EXPECT_NE((Color{1, 2, 3}), (Color{3, 2, 1}));
    }

    TEST(ThemeTests, OffersAtLeastOneTheme) {
        EXPECT_FALSE(availableThemes().empty());
    }

    TEST(ThemeTests, EveryThemeIsNamed) {
        for (const Theme& theme : availableThemes()) {
            EXPECT_FALSE(theme.name.empty());
        }
    }

    TEST(ThemeTests, NoTwoThemesShareAName) {
        std::unordered_set<std::string> names;
        for (const Theme& theme : availableThemes()) {
            names.insert(theme.name);
        }

        EXPECT_EQ(names.size(), availableThemes().size());
    }

    TEST(ThemeTests, TheDefaultIsOneOfTheThemesOffered) {
        bool isOffered = false;
        for (const Theme& theme : availableThemes()) {
            isOffered = isOffered || theme.name == defaultTheme().name;
        }

        EXPECT_TRUE(isOffered);
    }

    TEST(ThemeTests, FindsAThemeByName) {
        const std::string wanted = availableThemes().back().name;

        EXPECT_EQ(themeNamed(wanted).name, wanted);
    }

    TEST(ThemeTests, FallsBackToTheDefaultForAThemeThatIsNotThere) {
        EXPECT_EQ(themeNamed("no such theme").name, defaultTheme().name);
        EXPECT_EQ(themeNamed("").name, defaultTheme().name);
    }

    TEST(ThemeTests, EveryThemeSeparatesTextFromItsBackground) {
        for (const Theme& theme : availableThemes()) {
            EXPECT_NE(theme.text, theme.background) << theme.name;
        }
    }

    TEST(ThemeSelectionTests, StartsOnTheDefaultTheme) {
        const ThemeSelection selection;

        EXPECT_EQ(selection.current().name, defaultTheme().name);
    }

    TEST(ThemeSelectionTests, DressesEverythingInAThemeThatExists) {
        ThemeSelection selection;
        const std::string wanted = availableThemes().back().name;

        selection.change(wanted);

        EXPECT_EQ(selection.current().name, wanted);
    }

    TEST(ThemeSelectionTests, AThemeThatIsNotThereFallsBackToTheDefault) {
        ThemeSelection selection;

        selection.change("no such theme");

        EXPECT_EQ(selection.current().name, defaultTheme().name);
    }
}  // namespace cpp_warships::head::common
