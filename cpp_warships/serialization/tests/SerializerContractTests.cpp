#include <gtest/gtest.h>
#include <serialization/ISerializable.h>
#include <serialization/ISerializer.h>
#include <serialization/exceptions/DeserializationException.h>
#include <serialization/exceptions/InterpretationException.h>
#include <serialization/exceptions/SerializationException.h>
#include <serialization/helpers/TupleBuilder.h>

#include <string>
#include <tuple>
#include <type_traits>

namespace cpp_warships::serialization {
    namespace {
        inline char NAMED_TYPE[] = "Named";
        inline char CHILD_TYPE[] = "Child";

        class NamedThing final : public ISerializable<NAMED_TYPE> {};

        class ChildSerializer final : public ISerializer<std::string, int, CHILD_TYPE> {
        public:
            bool isRelated(std::string) override {
                return true;
            }

            std::string serialize(int& item) override {
                return std::to_string(item);
            }

            int deserialize(std::string) override {
                return marker;
            }

            int marker = 0;
        };

        class ParentSerializer final
            : public ISerializer<std::string, int, NAMED_TYPE, ChildSerializer> {
        public:
            bool isRelated(std::string) override {
                return true;
            }

            std::string serialize(int& item) override {
                return std::get<0>(childrenSerializers).serialize(item);
            }

            int deserialize(std::string data) override {
                return std::get<0>(childrenSerializers).deserialize(data);
            }
        };

        class UnnamedSerializable final : public ISerializableTyped<nullptr> {};
    }  // namespace

    TEST(SerializableTests, NamesItsType) {
        NamedThing thing;

        EXPECT_EQ(thing.getType(), "Named");
    }

    TEST(SerializableTests, AnUnnamedTypeRefusesToNameItself) {
        UnnamedSerializable thing;

        EXPECT_THROW((void)thing.getType(), exceptions::InterpretationException);
    }

    TEST(SerializableTests, RecognisesASerializableDerivative) {
        EXPECT_TRUE(is_serializable_derivative<NamedThing>::value);
        EXPECT_FALSE(is_serializable_derivative<int>::value);
    }

    TEST(SerializerTests, NamesItsType) {
        ChildSerializer serializer;

        EXPECT_EQ(serializer.getType(), "Child");
    }

    TEST(SerializerTests, RecognisesASerializerDerivative) {
        EXPECT_TRUE(is_serializer_derivative<ChildSerializer>::value);
        EXPECT_FALSE(is_serializer_derivative<NamedThing>::value);
    }

    TEST(SerializerTests, WiresUpAChildAtRuntime) {
        ParentSerializer parent;
        ChildSerializer child;
        child.marker = 7;

        parent.setChildrenSerializers(&child);

        EXPECT_EQ(parent.deserialize("anything"), 7);
    }

    TEST(SerializerTests, DelegatesSerializingToItsChild) {
        ParentSerializer parent;
        ChildSerializer child;
        parent.setChildrenSerializers(&child);
        int value = 12;

        EXPECT_EQ(parent.serialize(value), "12");
    }

    TEST(SerializerTests, WiringUpAgainReplacesTheChild) {
        ParentSerializer parent;
        ChildSerializer first;
        ChildSerializer second;
        first.marker = 1;
        second.marker = 2;

        parent.setChildrenSerializers(&first);
        parent.setChildrenSerializers(&second);

        EXPECT_EQ(parent.deserialize("anything"), 2);
    }

    TEST(TupleBuilderTests, PicksTheArgumentOfEachWantedType) {
        ChildSerializer child;
        child.marker = 5;

        const std::tuple<ChildSerializer> built =
            helpers::TupleBuilder<ChildSerializer>::build(child);

        EXPECT_EQ(std::get<0>(built).marker, 5);
    }

    TEST(TupleBuilderTests, DefaultConstructsWhenNothingIsGiven) {
        const std::tuple<ChildSerializer> built = helpers::TupleBuilder<ChildSerializer>::build();

        EXPECT_EQ(std::get<0>(built).marker, 0);
    }

    TEST(TupleBuilderTests, RecognisesATypeInAPack) {
        EXPECT_TRUE((helpers::is_one_of<int, float, int, char>::value));
        EXPECT_FALSE((helpers::is_one_of<double, float, int, char>::value));
    }

    TEST(ExceptionTests, ADeserializationErrorNamesTheTypeAndTheReason) {
        const exceptions::DeserializationException error{"Board", "it was not a board"};

        const std::string message = error.what();
        EXPECT_NE(message.find("Board"), std::string::npos);
        EXPECT_NE(message.find("it was not a board"), std::string::npos);
        EXPECT_NE(message.find("deserialization error"), std::string::npos);
    }

    TEST(ExceptionTests, ASerializationErrorNamesTheTypeAndTheReason) {
        const exceptions::SerializationException error{"Board", "it could not be written"};

        const std::string message = error.what();
        EXPECT_NE(message.find("Board"), std::string::npos);
        EXPECT_NE(message.find("it could not be written"), std::string::npos);
    }

    TEST(ExceptionTests, AnInterpretationErrorCarriesItsReason) {
        const exceptions::InterpretationException error{"the cast failed"};

        const std::string message = error.what();
        EXPECT_NE(message.find("the cast failed"), std::string::npos);
        EXPECT_NE(message.find("InterpretationException"), std::string::npos);
    }

    TEST(ExceptionTests, EveryErrorIsCaughtAsAStandardException) {
        try {
            throw exceptions::DeserializationException{"Board", "no"};
        } catch (const std::exception& error) {
            EXPECT_NE(std::string{error.what()}.find("Board"), std::string::npos);
            return;
        }

        FAIL() << "the exception was not thrown";
    }
}  // namespace cpp_warships::serialization
