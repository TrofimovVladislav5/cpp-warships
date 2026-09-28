#include <gtest/gtest.h>
#include <input_parser/CommandParser.h>
#include <input_parser/VoidParser.h>
#include <input_parser/builder/ConfigCommandBuilder.h>
#include <input_parser/builder/DefaultParameterBuilder.h>
#include <input_parser/command/ParserCommand.h>
#include <input_parser/model/Parser.h>
#include <input_parser/model/ParserCommandInfo.h>

#include <cstddef>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

namespace cpp_warships::input_parser {
    namespace {
        /** @brief Captures whatever is written to a stream while it is in scope. */
        class StreamCapture {
        public:
            explicit StreamCapture(std::ostream& stream)
                : stream_(stream)
                , original_(stream.rdbuf(captured_.rdbuf())) {}

            StreamCapture(const StreamCapture&) = delete;
            StreamCapture& operator=(const StreamCapture&) = delete;

            ~StreamCapture() {
                stream_.rdbuf(original_);
            }

            [[nodiscard]] std::string text() const {
                return captured_.str();
            }

        private:
            std::ostream& stream_;
            std::ostringstream captured_;
            std::streambuf* original_;
        };

        /** @brief A scheme of one command that notes when it ran. */
        model::SchemeMap<void> schemeRunning(bool& wasRun, std::string& seenValue) {
            builder::ConfigCommandBuilder<void> commandBuilder;
            builder::DefaultParameterBuilder parameterBuilder;

            const model::ParserParameter countParameter = parameterBuilder.addFlag("--count")
                                                              .setValidator(std::regex{"^[0-9]+$"})
                                                              .setNecessary(false)
                                                              .build();

            model::SchemeMap<void> scheme;
            scheme.insert(
                {"run",
                 model::ParserCommandInfo<void>{
                     commandBuilder.setDescription("runs a thing")
                         .addParameter(countParameter)
                         .setCallback([&wasRun, &seenValue](model::ParsedOptions options) {
                             wasRun = true;
                             const auto found = options.find("count");
                             seenValue = found == options.end() ? "" : found->second;
                         })
                         .buildAndReset()
                 }}
            );

            return scheme;
        }
    }  // namespace

    TEST(VoidParserTests, AddsAHelpCommandOfItsOwn) {
        bool wasRun = false;
        std::string seenValue;
        VoidParser parser{schemeRunning(wasRun, seenValue)};
        const StreamCapture capture{std::cout};

        EXPECT_NO_THROW(parser.executedParse("help"));

        EXPECT_NE(capture.text().find("supported commands"), std::string::npos);
    }

    TEST(VoidParserTests, RunsTheCommandItWasGiven) {
        bool wasRun = false;
        std::string seenValue;
        VoidParser parser{schemeRunning(wasRun, seenValue)};

        parser.executedParse("run");

        EXPECT_TRUE(wasRun);
    }

    TEST(VoidParserTests, PassesAValidatedOptionThrough) {
        bool wasRun = false;
        std::string seenValue;
        VoidParser parser{schemeRunning(wasRun, seenValue)};

        parser.executedParse("run --count 42");

        EXPECT_TRUE(wasRun);
        EXPECT_EQ(seenValue, "42");
    }

    TEST(VoidParserTests, RefusesACommandItDoesNotKnowWhenNoErrorCallbackWasGiven) {
        bool wasRun = false;
        std::string seenValue;
        VoidParser parser{schemeRunning(wasRun, seenValue)};

        EXPECT_THROW(parser.executedParse("nonsense"), std::invalid_argument);
        EXPECT_FALSE(wasRun);
    }

    TEST(VoidParserTests, ShowsTheErrorItWasGivenForACommandItDoesNotKnow) {
        bool wasRun = false;
        std::string seenValue;
        bool isErrorShown = false;
        VoidParser parser{schemeRunning(wasRun, seenValue), [&isErrorShown](model::ParsedOptions) {
                              isErrorShown = true;
                          }};

        parser.executedParse("nonsense");

        EXPECT_TRUE(isErrorShown);
        EXPECT_FALSE(wasRun);
    }

    TEST(VoidParserTests, BindingHoldsTheCommandUntilItIsCalled) {
        bool wasRun = false;
        std::string seenValue;
        VoidParser parser{schemeRunning(wasRun, seenValue)};

        const model::BindedParseCallback<void> bound = parser.bindedParse("run --count 7");
        EXPECT_FALSE(wasRun);

        bound();

        EXPECT_TRUE(wasRun);
        EXPECT_EQ(seenValue, "7");
    }

    TEST(VoidParserTests, RefusesAnEmptyInput) {
        bool wasRun = false;
        std::string seenValue;
        bool isErrorShown = false;
        VoidParser parser{schemeRunning(wasRun, seenValue), [&isErrorShown](model::ParsedOptions) {
                              isErrorShown = true;
                          }};

        parser.executedParse("");

        EXPECT_TRUE(isErrorShown);
        EXPECT_FALSE(wasRun);
    }

    TEST(VoidParserTests, UsesAHelpCallbackWhenOneIsGiven) {
        bool wasRun = false;
        std::string seenValue;
        bool isHelpShown = false;
        VoidParser parser{
            schemeRunning(wasRun, seenValue),
            [](model::ParsedOptions) {},
            [&isHelpShown](model::SchemeMap<void>) { isHelpShown = true; }
        };

        parser.executedParse("help");

        EXPECT_TRUE(isHelpShown);
    }

    TEST(VoidParserTests, KeepsASchemeThatAlreadyHasItsOwnHelp) {
        bool wasRun = false;
        std::string seenValue;
        bool isOwnHelpRun = false;
        model::SchemeMap<void> scheme = schemeRunning(wasRun, seenValue);
        builder::ConfigCommandBuilder<void> commandBuilder;
        scheme.insert(
            {"help",
             model::ParserCommandInfo<void>{commandBuilder.setDescription("our own help")
                                                .setCallback([&isOwnHelpRun](model::ParsedOptions) {
                                                    isOwnHelpRun = true;
                                                })
                                                .buildAndReset()}}
        );
        VoidParser parser{scheme};

        parser.executedParse("help");

        EXPECT_TRUE(isOwnHelpRun);
    }

    namespace {
        /** @brief A command scheme whose one command notes when it ran. */
        model::SchemeMap<command::ParserCommand*> commandSchemeRunning(bool& wasRun) {
            builder::ConfigCommandBuilder<command::ParserCommand*> commandBuilder;

            model::SchemeMap<command::ParserCommand*> scheme;
            scheme.insert(
                {"run",
                 model::ParserCommandInfo<command::ParserCommand*>{
                     commandBuilder.setDescription("runs a thing")
                         .setCallback([&wasRun](model::ParsedOptions) {
                             wasRun = true;
                             return static_cast<command::ParserCommand*>(nullptr);
                         })
                         .buildAndReset()
                 }}
            );

            return scheme;
        }
    }  // namespace

    TEST(CommandParserTests, AddsAHelpCommandOfItsOwn) {
        bool wasRun = false;
        CommandParser parser{commandSchemeRunning(wasRun), [](model::ParsedOptions) {}};
        const StreamCapture capture{std::cout};

        parser.executedParse("help");

        EXPECT_NE(capture.text().find("supported commands"), std::string::npos);
    }

    TEST(CommandParserTests, UsesTheHelpCallbackItWasGiven) {
        bool wasRun = false;
        bool isOwnHelpShown = false;
        CommandParser parser{
            commandSchemeRunning(wasRun),
            [](model::ParsedOptions) {},
            [&isOwnHelpShown](model::SchemeMap<command::ParserCommand*>) { isOwnHelpShown = true; }
        };
        const StreamCapture capture{std::cout};

        parser.executedParse("help");

        EXPECT_TRUE(isOwnHelpShown);
        EXPECT_EQ(capture.text().find("supported commands"), std::string::npos);
    }

    TEST(CommandParserTests, TheHelpCallbackIsShownTheWholeSchemeIncludingHelpItself) {
        bool wasRun = false;
        std::size_t describedCount = 0;
        CommandParser parser{
            commandSchemeRunning(wasRun),
            [](model::ParsedOptions) {},
            [&describedCount](model::SchemeMap<command::ParserCommand*> scheme) {
                describedCount = scheme.size();
            }
        };

        parser.executedParse("help");

        EXPECT_EQ(describedCount, 2U);
    }
}  // namespace cpp_warships::input_parser
