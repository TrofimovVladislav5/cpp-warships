#include <application/persistence/MemorySaveStorage.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

namespace cpp_warships::persistence {
    TEST(MemorySaveStorageTests, StartsWithNothingStored) {
        const MemorySaveStorage storage;

        EXPECT_TRUE(storage.list().empty());
        EXPECT_FALSE(storage.contains("anything"));
        EXPECT_EQ(storage.read("anything"), std::nullopt);
    }

    TEST(MemorySaveStorageTests, ReadsBackWhatItWrote) {
        MemorySaveStorage storage;

        EXPECT_TRUE(storage.write("first", "contents"));

        EXPECT_TRUE(storage.contains("first"));
        EXPECT_EQ(storage.read("first"), "contents");
    }

    TEST(MemorySaveStorageTests, WritingAgainReplacesTheSave) {
        MemorySaveStorage storage;
        storage.write("first", "old");

        storage.write("first", "new");

        EXPECT_EQ(storage.read("first"), "new");
        EXPECT_EQ(storage.list().size(), 1U);
    }

    TEST(MemorySaveStorageTests, ListsEverySaveHeld) {
        MemorySaveStorage storage;
        storage.write("alpha", "a");
        storage.write("beta", "b");

        std::vector<std::string> names = storage.list();
        std::sort(names.begin(), names.end());

        EXPECT_EQ(names, (std::vector<std::string>{"alpha", "beta"}));
    }

    TEST(MemorySaveStorageTests, RemovesASaveThatIsThere) {
        MemorySaveStorage storage;
        storage.write("first", "contents");

        EXPECT_TRUE(storage.remove("first"));

        EXPECT_FALSE(storage.contains("first"));
        EXPECT_TRUE(storage.list().empty());
    }

    TEST(MemorySaveStorageTests, RemovingWhatIsNotThereReportsSo) {
        MemorySaveStorage storage;

        EXPECT_FALSE(storage.remove("missing"));
    }

    TEST(MemorySaveStorageTests, KeepsAnEmptySaveApartFromAMissingOne) {
        MemorySaveStorage storage;
        storage.write("empty", "");

        EXPECT_TRUE(storage.contains("empty"));
        EXPECT_EQ(storage.read("empty"), "");
        EXPECT_EQ(storage.read("missing"), std::nullopt);
    }

    TEST(MemorySaveStorageTests, IsUsableThroughTheStorageInterface) {
        MemorySaveStorage concrete;
        SaveStorage& storage = concrete;

        storage.write("through-interface", "contents");

        EXPECT_EQ(storage.read("through-interface"), "contents");
    }
}  // namespace cpp_warships::persistence
