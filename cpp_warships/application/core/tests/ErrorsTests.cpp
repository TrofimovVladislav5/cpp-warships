#include <application/core/errors/ErrorLayer.h>
#include <application/core/errors/WarshipsException.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <iterator>
#include <string>
#include <string_view>

namespace cpp_warships::core::errors {
    namespace {
        /** @brief Stands in for a layer's own base exception, whose constructor is protected. */
        class TestException : public WarshipsException {
        public:
            TestException(ErrorLayer layer, const std::string& message)
                : WarshipsException(layer, message) {}
        };
    }  // namespace

    TEST(ErrorLayerTests, NamesEveryLayer) {
        EXPECT_EQ(nameOf(ErrorLayer::Core), "core");
        EXPECT_EQ(nameOf(ErrorLayer::Flow), "flow");
        EXPECT_EQ(nameOf(ErrorLayer::Persistence), "persistence");
        EXPECT_EQ(nameOf(ErrorLayer::Model), "model");
        EXPECT_EQ(nameOf(ErrorLayer::Presentation), "presentation");
    }

    TEST(ErrorLayerTests, GivesEveryLayerItsOwnName) {
        const std::string_view names[]{
            nameOf(ErrorLayer::Core),
            nameOf(ErrorLayer::Flow),
            nameOf(ErrorLayer::Persistence),
            nameOf(ErrorLayer::Model),
            nameOf(ErrorLayer::Presentation),
        };

        for (std::size_t left = 0; left < std::size(names); ++left) {
            for (std::size_t right = left + 1; right < std::size(names); ++right) {
                EXPECT_NE(names[left], names[right]);
            }
        }
    }

    TEST(WarshipsExceptionTests, RemembersTheLayerItCameFrom) {
        const TestException error{ErrorLayer::Flow, "nothing to do"};

        EXPECT_EQ(error.layer(), ErrorLayer::Flow);
    }

    TEST(WarshipsExceptionTests, PutsTheLayerInFrontOfTheMessage) {
        const TestException error{ErrorLayer::Persistence, "cannot write"};

        EXPECT_EQ(std::string{error.what()}, "persistence error: cannot write");
    }

    TEST(WarshipsExceptionTests, IsCaughtAsAStandardException) {
        try {
            throw TestException{ErrorLayer::Core, "bad cell"};
        } catch (const std::exception& error) {
            EXPECT_EQ(std::string{error.what()}, "core error: bad cell");
            return;
        }

        FAIL() << "the exception was not thrown";
    }

    TEST(WarshipsExceptionTests, IsCaughtAsAWarshipsException) {
        try {
            throw TestException{ErrorLayer::Model, "no match"};
        } catch (const WarshipsException& error) {
            EXPECT_EQ(error.layer(), ErrorLayer::Model);
            return;
        }

        FAIL() << "the exception was not thrown";
    }
}  // namespace cpp_warships::core::errors
