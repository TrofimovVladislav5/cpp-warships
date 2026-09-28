#include <gtest/gtest.h>
#include <utilities/TypesHelper.h>

#include <functional>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
    /** @brief Something with methods to be turned into callables. */
    class Counter {
    public:
        void increment() {
            ++count;
        }

        void addTo(int amount) {
            count += amount;
        }

        int addAndReport(int amount) {
            count += amount;
            return count;
        }

        int sumOf(int left, int right) {
            count = left + right;
            return count;
        }

        int count = 0;
    };
}  // namespace

TEST(TypesHelperTests, ReadsACellAsALetterAndANumber) {
    EXPECT_EQ(TypesHelper::cell("A1"), (std::pair<int, int>{0, 0}));
    EXPECT_EQ(TypesHelper::cell("C5"), (std::pair<int, int>{2, 4}));
    EXPECT_EQ(TypesHelper::cell("Z26"), (std::pair<int, int>{25, 25}));
}

TEST(TypesHelperTests, RefusesACellThatIsTooShort) {
    EXPECT_THROW((void)TypesHelper::cell("A"), std::invalid_argument);
    EXPECT_THROW((void)TypesHelper::cell(""), std::invalid_argument);
}

TEST(TypesHelperTests, RefusesACellNotStartingWithACapitalLetter) {
    EXPECT_THROW((void)TypesHelper::cell("a1"), std::invalid_argument);
    EXPECT_THROW((void)TypesHelper::cell("11"), std::invalid_argument);
}

TEST(TypesHelperTests, ConvertsToAPairTheSameWay) {
    EXPECT_EQ(TypesHelper::convertToPair("B3"), (std::pair<int, int>{1, 2}));
    EXPECT_THROW((void)TypesHelper::convertToPair("b3"), std::invalid_argument);
    EXPECT_THROW((void)TypesHelper::convertToPair("B"), std::invalid_argument);
}

TEST(TypesHelperTests, TurnsAMethodTakingNothingIntoACallable) {
    Counter counter;

    const std::function<void()> call = TypesHelper::methodToFunction(&Counter::increment, &counter);
    call();
    call();

    EXPECT_EQ(counter.count, 2);
}

TEST(TypesHelperTests, TurnsAMethodTakingOneArgumentIntoACallable) {
    Counter counter;

    const std::function<void(int)> call = TypesHelper::methodToFunction(&Counter::addTo, &counter);
    call(5);

    EXPECT_EQ(counter.count, 5);
}

TEST(TypesHelperTests, TurnsAMethodThatReportsBackIntoACallable) {
    Counter counter;

    const std::function<int(int)> call =
        TypesHelper::methodToFunction(&Counter::addAndReport, &counter);

    EXPECT_EQ(call(4), 4);
    EXPECT_EQ(call(3), 7);
}

TEST(TypesHelperTests, TurnsAMethodOfSeveralArgumentsIntoACallable) {
    Counter counter;

    const std::function<int(int, int)> call =
        TypesHelper::methodToFunction<Counter, int, int, int>(&Counter::sumOf, &counter);

    EXPECT_EQ(call(2, 3), 5);
    EXPECT_EQ(counter.count, 5);
}
