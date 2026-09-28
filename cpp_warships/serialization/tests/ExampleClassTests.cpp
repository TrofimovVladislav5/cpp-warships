#include <gtest/gtest.h>
#include <serialization/SerializerAggregator.h>
#include <serialization/example/ImplicitTestClass.h>
#include <serialization/example/TestClass.h>

#include <sstream>
#include <string>

namespace cpp_warships::serialization::example {
    namespace {
        /** @brief A test class serializer with its child wired up. */
        class WiredTestClassSerializer {
        public:
            WiredTestClassSerializer() {
                serializer.setChildrenSerializers(&implicitSerializer);
            }

            TestClassStringSerializer serializer;

        private:
            ImplicitTestClassStringSerializer implicitSerializer;
        };
    }  // namespace

    TEST(ImplicitTestClassTests, StartsAtItsDefaults) {
        const ImplicitTestClass item;

        EXPECT_EQ(item.stringPublicField, "default-public");
    }

    TEST(ImplicitTestClassTests, NamesItsType) {
        ImplicitTestClass item;

        EXPECT_EQ(item.getType(), "ImplicitTestClass");
    }

    TEST(ImplicitTestClassTests, ReadsBackWhatItWrote) {
        ImplicitTestClassStringSerializer serializer;
        ImplicitTestClass original;
        original.stringPublicField = "a value";

        const ImplicitTestClass restored = serializer.deserialize(serializer.serialize(original));

        EXPECT_EQ(restored.stringPublicField, "a value");
    }

    TEST(ImplicitTestClassTests, RecognisesItsOwnWriting) {
        ImplicitTestClassStringSerializer serializer;
        ImplicitTestClass item;

        EXPECT_TRUE(serializer.isRelated(serializer.serialize(item)));
        EXPECT_FALSE(serializer.isRelated("something else entirely"));
    }

    TEST(ImplicitTestClassTests, WritesItselfToAStream) {
        const ImplicitTestClass item;
        std::ostringstream written;

        written << item;

        EXPECT_NE(written.str().find("default-public"), std::string::npos);
    }

    TEST(TestClassTests, StartsAtItsDefaults) {
        const TestClass item;

        EXPECT_EQ(item.intPublicField, 0);
        EXPECT_EQ(item.stringPublicField, "default");
    }

    TEST(TestClassTests, NamesItsType) {
        TestClass item;

        EXPECT_EQ(item.getType(), "TestClass");
    }

    TEST(TestClassTests, RecognisesItsOwnWriting) {
        WiredTestClassSerializer wired;
        TestClass item;

        EXPECT_TRUE(wired.serializer.isRelated(wired.serializer.serialize(item)));
        EXPECT_FALSE(wired.serializer.isRelated("something else entirely"));
    }

    TEST(TestClassTests, ReadsBackItsOwnFields) {
        WiredTestClassSerializer wired;
        TestClass original;
        original.intPublicField = 17;
        original.stringPublicField = "a value";

        const TestClass restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(restored.intPublicField, 17);
        EXPECT_EQ(restored.stringPublicField, "a value");
    }

    TEST(TestClassTests, ReadsBackTheClassNestedInsideIt) {
        WiredTestClassSerializer wired;
        TestClass original;
        original.implicitClass.stringPublicField = "nested value";

        const TestClass restored =
            wired.serializer.deserialize(wired.serializer.serialize(original));

        EXPECT_EQ(restored.implicitClass.stringPublicField, "nested value");
    }

    TEST(TestClassTests, WritesItselfToAStream) {
        TestClass item;
        item.intPublicField = 5;
        std::ostringstream written;

        written << item;

        EXPECT_NE(written.str().find("TestClass"), std::string::npos);
        EXPECT_NE(written.str().find('5'), std::string::npos);
    }

    TEST(SerializerAggregatorTests, WritesAnItemThroughTheSerializerThatKnowsIt) {
        SerializerAggregator<std::string> aggregator;
        auto* implicitSerializer = new ImplicitTestClassStringSerializer();
        aggregator.setSerializers(implicitSerializer);
        ImplicitTestClass item;
        item.stringPublicField = "a value";

        const std::string written = aggregator.serialize(item);

        EXPECT_NE(written.find("a value"), std::string::npos);
    }

    TEST(SerializerAggregatorTests, ReadsAnItemBackThroughTheSerializerThatClaimsIt) {
        SerializerAggregator<std::string> aggregator;
        auto* implicitSerializer = new ImplicitTestClassStringSerializer();
        aggregator.setSerializers(implicitSerializer);
        ImplicitTestClass original;
        original.stringPublicField = "a value";
        const std::string written = aggregator.serialize(original);

        const auto restored = aggregator.deserialize<ImplicitTestClass>(written);

        EXPECT_EQ(restored.stringPublicField, "a value");
    }

    TEST(SerializerAggregatorTests, RefusesToWriteSomethingNoSerializerKnows) {
        SerializerAggregator<std::string> aggregator;
        TestClass item;

        EXPECT_THROW((void)aggregator.serialize(item), exceptions::SerializationException);
    }

    TEST(SerializerAggregatorTests, RefusesToReadSomethingNoSerializerClaims) {
        SerializerAggregator<std::string> aggregator;

        EXPECT_THROW(
            (void)aggregator.deserialize<ImplicitTestClass>("not a serialized thing"),
            exceptions::DeserializationException
        );
    }
}  // namespace cpp_warships::serialization::example
