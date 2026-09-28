#include <application/persistence/FilesystemSaveStorage.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace cpp_warships::persistence {
    namespace {
        /** @brief A directory of its own for one test, removed when the test ends. */
        class TemporaryDirectory {
        public:
            TemporaryDirectory() {
                const testing::TestInfo* test =
                    testing::UnitTest::GetInstance()->current_test_info();
                path_ = std::filesystem::temp_directory_path() /
                        ("warships-" + std::string{test->name()});
                std::filesystem::remove_all(path_);
                std::filesystem::create_directories(path_);
            }

            TemporaryDirectory(const TemporaryDirectory&) = delete;
            TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;

            ~TemporaryDirectory() {
                std::error_code error;
                std::filesystem::remove_all(path_, error);
            }

            [[nodiscard]] std::string path() const {
                return path_.string();
            }

        private:
            std::filesystem::path path_;
        };
    }  // namespace

    TEST(FilesystemSaveStorageTests, RemembersWhereItKeepsSaves) {
        const FilesystemSaveStorage storage{"/some/where"};

        EXPECT_EQ(storage.directoryPath(), "/some/where");
    }

    TEST(FilesystemSaveStorageTests, RootsItselfAtTheWorkingDirectory) {
        const FilesystemSaveStorage storage = FilesystemSaveStorage::inCurrentDirectory();

        EXPECT_EQ(storage.directoryPath(), std::filesystem::current_path().string());
    }

    TEST(FilesystemSaveStorageTests, AnEmptyDirectoryHoldsNoSaves) {
        const TemporaryDirectory directory;
        const FilesystemSaveStorage storage{directory.path()};

        EXPECT_TRUE(storage.list().empty());
        EXPECT_FALSE(storage.contains("missing"));
        EXPECT_EQ(storage.read("missing"), std::nullopt);
    }

    TEST(FilesystemSaveStorageTests, ADirectoryThatIsNotThereHoldsNoSaves) {
        const FilesystemSaveStorage storage{"/no/such/directory/warships"};

        EXPECT_TRUE(storage.list().empty());
        EXPECT_FALSE(storage.contains("anything"));
    }

    TEST(FilesystemSaveStorageTests, ReadsBackWhatItWrote) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};

        EXPECT_TRUE(storage.write("first", "{\"a\": 1}"));

        EXPECT_TRUE(storage.contains("first"));
        EXPECT_EQ(storage.read("first"), "{\"a\": 1}");
    }

    TEST(FilesystemSaveStorageTests, WritesSavesAsJsonFilesNamedAfterTheSave) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};

        storage.write("20260928-120000", "contents");

        const std::filesystem::path expected =
            std::filesystem::path{directory.path()} / "20260928-120000.json";
        EXPECT_TRUE(std::filesystem::is_regular_file(expected));
    }

    TEST(FilesystemSaveStorageTests, CreatesTheDirectoryOnFirstWrite) {
        const TemporaryDirectory directory;
        const std::string nested = (std::filesystem::path{directory.path()} / "deeper").string();
        FilesystemSaveStorage storage{nested};

        EXPECT_TRUE(storage.write("first", "contents"));

        EXPECT_TRUE(std::filesystem::is_directory(nested));
    }

    TEST(FilesystemSaveStorageTests, WritingAgainReplacesTheSave) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};
        storage.write("first", "old");

        storage.write("first", "new");

        EXPECT_EQ(storage.read("first"), "new");
        EXPECT_EQ(storage.list().size(), 1U);
    }

    TEST(FilesystemSaveStorageTests, ListsSavesByNameWithoutTheExtension) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};
        storage.write("alpha", "a");
        storage.write("beta", "b");

        std::vector<std::string> names = storage.list();
        std::sort(names.begin(), names.end());

        EXPECT_EQ(names, (std::vector<std::string>{"alpha", "beta"}));
    }

    TEST(FilesystemSaveStorageTests, IgnoresFilesThatAreNotSaves) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};
        storage.write("alpha", "a");
        std::ofstream stray{std::filesystem::path{directory.path()} / "notes.txt"};
        stray << "not a save";
        stray.close();

        EXPECT_EQ(storage.list(), (std::vector<std::string>{"alpha"}));
    }

    TEST(FilesystemSaveStorageTests, RemovesASaveThatIsThere) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};
        storage.write("first", "contents");

        EXPECT_TRUE(storage.remove("first"));

        EXPECT_FALSE(storage.contains("first"));
        EXPECT_TRUE(storage.list().empty());
    }

    TEST(FilesystemSaveStorageTests, RemovingWhatIsNotThereReportsSo) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};

        EXPECT_FALSE(storage.remove("missing"));
    }

    TEST(FilesystemSaveStorageTests, RefusesANameThatWouldClimbOutOfTheDirectory) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};

        EXPECT_FALSE(storage.write("../escaped", "contents"));
        EXPECT_FALSE(storage.write("../../escaped", "contents"));
        EXPECT_FALSE(storage.write("nested/escaped", "contents"));

        const std::filesystem::path parent = std::filesystem::path{directory.path()}.parent_path();
        EXPECT_FALSE(std::filesystem::exists(parent / "escaped.json"));
        EXPECT_TRUE(storage.list().empty());
    }

    TEST(FilesystemSaveStorageTests, RefusesAnAbsoluteName) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};
        const std::filesystem::path elsewhere =
            std::filesystem::temp_directory_path() / "warships-should-not-exist";

        EXPECT_FALSE(storage.write(elsewhere.string(), "contents"));

        EXPECT_FALSE(std::filesystem::exists(elsewhere.string() + ".json"));
    }

    TEST(FilesystemSaveStorageTests, RefusesToReadOrRemoveThroughAClimbingName) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};
        const std::filesystem::path outside =
            std::filesystem::path{directory.path()}.parent_path() / "warships-outsider.json";
        std::ofstream planted{outside};
        planted << "not a save of ours";
        planted.close();

        EXPECT_EQ(storage.read("../warships-outsider"), std::nullopt);
        EXPECT_FALSE(storage.contains("../warships-outsider"));
        EXPECT_FALSE(storage.remove("../warships-outsider"));

        EXPECT_TRUE(std::filesystem::exists(outside));
        std::filesystem::remove(outside);
    }

    TEST(FilesystemSaveStorageTests, RefusesTheEmptyAndDottedNames) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};

        for (const std::string& name : {std::string{}, std::string{"."}, std::string{".."}}) {
            EXPECT_FALSE(storage.write(name, "contents")) << "name '" << name << "'";
            EXPECT_FALSE(storage.contains(name)) << "name '" << name << "'";
        }
    }

    TEST(FilesystemSaveStorageTests, StillAcceptsAnOrdinarySaveIdentifier) {
        const TemporaryDirectory directory;
        FilesystemSaveStorage storage{directory.path()};

        EXPECT_TRUE(storage.write("cc55eebd-cb32-4d5b-ae72-cf479c990f42", "contents"));
        EXPECT_TRUE(storage.write("20260928-021655", "contents"));

        EXPECT_EQ(storage.list().size(), 2U);
    }

    TEST(FilesystemSaveStorageTests, SurvivesAReopeningOfTheSameDirectory) {
        const TemporaryDirectory directory;
        {
            FilesystemSaveStorage writer{directory.path()};
            writer.write("kept", "contents");
        }

        const FilesystemSaveStorage reader{directory.path()};

        EXPECT_EQ(reader.read("kept"), "contents");
    }
}  // namespace cpp_warships::persistence
