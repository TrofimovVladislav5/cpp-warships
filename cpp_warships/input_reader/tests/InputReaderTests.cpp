#include <gtest/gtest.h>
#include <input_reader/InputReader.h>
#include <input_reader/command_reader/CommandInputReader.h>
#include <input_reader/config_reader/ConfigInputReader.h>
#include <input_reader/console_reader/ConsoleInputReader.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace cpp_warships::input_reader {
    namespace {
        /** @brief Answers reads from the console with prepared lines while it is in scope. */
        class ConsoleInput {
        public:
            explicit ConsoleInput(const std::string& lines)
                : prepared_(lines)
                , original_(std::cin.rdbuf(prepared_.rdbuf())) {}

            ConsoleInput(const ConsoleInput&) = delete;
            ConsoleInput& operator=(const ConsoleInput&) = delete;

            ~ConsoleInput() {
                std::cin.rdbuf(original_);
            }

        private:
            std::istringstream prepared_;
            std::streambuf* original_;
        };

        /** @brief Swallows whatever is written to a stream while it is in scope. */
        class SilencedStream {
        public:
            explicit SilencedStream(std::ostream& stream)
                : stream_(stream)
                , original_(stream.rdbuf(swallowed_.rdbuf())) {}

            SilencedStream(const SilencedStream&) = delete;
            SilencedStream& operator=(const SilencedStream&) = delete;

            ~SilencedStream() {
                stream_.rdbuf(original_);
            }

        private:
            std::ostream& stream_;
            std::ostringstream swallowed_;
            std::streambuf* original_;
        };

        /** @brief A file of its own for one test, removed when the test ends. */
        class TemporaryFile {
        public:
            explicit TemporaryFile(const std::string& contents) {
                const testing::TestInfo* test =
                    testing::UnitTest::GetInstance()->current_test_info();
                path_ = std::filesystem::temp_directory_path() /
                        ("warships-reader-" + std::string{test->name()} + ".txt");

                std::ofstream file{path_};
                file << contents;
            }

            TemporaryFile(const TemporaryFile&) = delete;
            TemporaryFile& operator=(const TemporaryFile&) = delete;

            ~TemporaryFile() {
                std::error_code error;
                std::filesystem::remove(path_, error);
            }

            [[nodiscard]] std::string path() const {
                return path_.string();
            }

        private:
            std::filesystem::path path_;
        };
    }  // namespace

    TEST(ConsoleInputReaderTests, ReadsALineAsItWasTyped) {
        const ConsoleInput input{"a typed command\n"};
        console_reader::ConsoleInputReader reader;

        EXPECT_EQ(reader.readCommand(), "a typed command");
    }

    TEST(ConsoleInputReaderTests, ReadsOneLineAtATime) {
        const ConsoleInput input{"first\nsecond\n"};
        console_reader::ConsoleInputReader reader;

        EXPECT_EQ(reader.readCommand(), "first");
        EXPECT_EQ(reader.readCommand(), "second");
    }

    TEST(ConsoleInputReaderTests, ReadsAnEmptyLineAsNothing) {
        const ConsoleInput input{"\n"};
        console_reader::ConsoleInputReader reader;

        EXPECT_EQ(reader.readCommand(), "");
    }

    TEST(ConsoleInputReaderTests, IsUsableThroughTheReaderInterface) {
        const ConsoleInput input{"through the interface\n"};
        console_reader::ConsoleInputReader concrete;
        InputReader<>& reader = concrete;

        EXPECT_EQ(reader.readCommand(), "through the interface");
    }

    TEST(CommandInputReaderTests, TranslatesAKeyIntoWhatItStandsFor) {
        const TemporaryFile keymap{"s;start the game\nq;quit\n"};
        command_reader::CommandInputReader reader{keymap.path()};
        const ConsoleInput input{"s\n"};

        EXPECT_EQ(reader.readCommand(), "start the game");
    }

    TEST(CommandInputReaderTests, PassesThroughWhatItHasNoKeyFor) {
        const TemporaryFile keymap{"s;start the game\n"};
        command_reader::CommandInputReader reader{keymap.path()};
        const ConsoleInput input{"something else\n"};

        EXPECT_EQ(reader.readCommand(), "something else");
    }

    TEST(CommandInputReaderTests, TrimsSpacesAroundBothSidesOfAKeymapLine) {
        const TemporaryFile keymap{"  s  ;  start the game  \n"};
        command_reader::CommandInputReader reader{keymap.path()};
        const ConsoleInput input{"s\n"};

        EXPECT_EQ(reader.readCommand(), "start the game");
    }

    TEST(CommandInputReaderTests, IgnoresALineThatIsNotAPair) {
        const TemporaryFile keymap{"s;start\nnot a pair\na;b;c\n"};
        command_reader::CommandInputReader reader{keymap.path()};
        const ConsoleInput input{"not a pair\n"};

        EXPECT_EQ(reader.readCommand(), "not a pair");
    }

    TEST(CommandInputReaderTests, TakesTheDelimiterItWasGiven) {
        const TemporaryFile keymap{"s=start the game\n"};
        command_reader::CommandInputReader reader{keymap.path(), '='};
        const ConsoleInput input{"s\n"};

        EXPECT_EQ(reader.readCommand(), "start the game");
    }

    TEST(CommandInputReaderTests, ComplainsAboutAFileThatIsNotThereAndReadsOnRegardless) {
        const SilencedStream silenced{std::cerr};
        command_reader::CommandInputReader reader{"/no/such/file/keymap.txt"};
        const ConsoleInput input{"anything\n"};

        EXPECT_EQ(reader.readCommand(), "anything");
    }

    TEST(CommandInputReaderTests, ComplainsAboutADirectoryGivenAsAFile) {
        const SilencedStream silenced{std::cerr};
        const std::string directory = std::filesystem::temp_directory_path().string();

        EXPECT_NO_THROW(command_reader::CommandInputReader{directory});
    }

    TEST(ConfigInputReaderTests, ReadsTheFileLineByLine) {
        const TemporaryFile config{"first command\nsecond command\n"};
        config_reader::ConfigInputReader reader{config.path()};

        EXPECT_EQ(reader.readCommand(), "first command");
        EXPECT_EQ(reader.readCommand(), "second command");
    }

    TEST(ConfigInputReaderTests, FallsBackToTheConsoleOnceTheFileRunsOut) {
        const TemporaryFile config{"the only line\n"};
        config_reader::ConfigInputReader reader{config.path()};
        const ConsoleInput input{"typed afterwards\n"};

        EXPECT_EQ(reader.readCommand(), "the only line");
        EXPECT_EQ(reader.readCommand(), "typed afterwards");
    }

    TEST(ConfigInputReaderTests, AnEmptyFileGoesStraightToTheConsole) {
        const TemporaryFile config{""};
        config_reader::ConfigInputReader reader{config.path()};
        const ConsoleInput input{"typed instead\n"};

        EXPECT_EQ(reader.readCommand(), "typed instead");
    }

    TEST(ConfigInputReaderTests, ComplainsAboutAFileThatIsNotThereAndReadsOnRegardless) {
        const SilencedStream silenced{std::cerr};
        config_reader::ConfigInputReader reader{"/no/such/file/config.txt"};
        const ConsoleInput input{"typed instead\n"};

        EXPECT_EQ(reader.readCommand(), "typed instead");
    }

    TEST(ConfigInputReaderTests, KeepsBlankLinesOfTheFile) {
        const TemporaryFile config{"first\n\nthird\n"};
        config_reader::ConfigInputReader reader{config.path()};

        EXPECT_EQ(reader.readCommand(), "first");
        EXPECT_EQ(reader.readCommand(), "");
        EXPECT_EQ(reader.readCommand(), "third");
    }

    TEST(ConfigInputReaderTests, IsUsableThroughTheReaderInterface) {
        const TemporaryFile config{"from the file\n"};
        config_reader::ConfigInputReader concrete{config.path()};
        InputReader<>& reader = concrete;

        EXPECT_EQ(reader.readCommand(), "from the file");
    }
}  // namespace cpp_warships::input_reader
