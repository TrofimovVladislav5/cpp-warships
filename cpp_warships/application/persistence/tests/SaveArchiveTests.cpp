#include <application/core/Board.h>
#include <application/core/Direction.h>
#include <application/core/MatchSettings.h>
#include <application/flow/AiOpponent.h>
#include <application/flow/MatchEvent.h>
#include <application/flow/MatchPhase.h>
#include <application/flow/Participant.h>
#include <application/flow/SkillKind.h>
#include <application/persistence/MatchSnapshot.h>
#include <application/persistence/MemorySaveStorage.h>
#include <application/persistence/SaveArchive.h>
#include <application/persistence/SaveSummary.h>
#include <gtest/gtest.h>

#include <deque>
#include <nlohmann/json.hpp>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace cpp_warships::persistence {
    namespace {
        constexpr int BOARD_SIZE = 8;

        MatchSnapshot makeSnapshot(int roundNumber = 1) {
            core::Board playerBoard{BOARD_SIZE, BOARD_SIZE};
            playerBoard.place({0, 0}, core::Direction::Horizontal, 2, 2);

            return MatchSnapshot{
                core::MatchSettings::forBoardSize(BOARD_SIZE),
                std::move(playerBoard),
                core::Board{BOARD_SIZE, BOARD_SIZE},
                std::deque<flow::SkillKind>{flow::SkillKind::Scanner},
                roundNumber,
                flow::MatchPhase::Battle,
                flow::Participant::Player,
                false,
                flow::AiMemory{},
                flow::MatchEventLog{}
            };
        }

        /** @brief A storage whose writes always fail, as a full disk would. */
        class UnwritableStorage final : public SaveStorage {
        public:
            [[nodiscard]] std::vector<std::string> list() const override {
                return {};
            }

            [[nodiscard]] std::optional<std::string> read(const std::string&) const override {
                return std::nullopt;
            }

            bool write(const std::string&, const std::string&) override {
                return false;
            }

            [[nodiscard]] bool contains(const std::string&) const override {
                return false;
            }

            bool remove(const std::string&) override {
                return false;
            }
        };

        /** @brief Writes a save and then back-dates it, so ordering can be pinned down. */
        void saveMadeAt(
            MemorySaveStorage& storage,
            SaveArchive& archive,
            const std::string& id,
            const std::string& name,
            const std::string& timestamp
        ) {
            archive.save(id, name, makeSnapshot());

            nlohmann::json document = nlohmann::json::parse(*storage.read(id));
            document["meta"]["timestamp"] = timestamp;
            storage.write(id, document.dump());
        }
    }  // namespace

    TEST(SaveArchiveTests, AnEmptyStorageHoldsNoSaves) {
        MemorySaveStorage storage;
        const SaveArchive archive{storage};

        EXPECT_TRUE(archive.listSaves().empty());
    }

    TEST(SaveArchiveTests, SavesAndLoadsAMatch) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};

        ASSERT_TRUE(archive.save("a-save", "my game", makeSnapshot(5)));

        const std::optional<MatchSnapshot> loaded = archive.load("a-save");
        ASSERT_TRUE(loaded.has_value());
        EXPECT_EQ(loaded->roundNumber(), 5);
        EXPECT_EQ(loaded->phase(), flow::MatchPhase::Battle);
    }

    TEST(SaveArchiveTests, WritesTheMatchUnderDataAndTheRestUnderMeta) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};

        archive.save("a-save", "my game", makeSnapshot());

        const nlohmann::json document = nlohmann::json::parse(*storage.read("a-save"));
        ASSERT_TRUE(document.contains("data"));
        ASSERT_TRUE(document.contains("meta"));
        EXPECT_EQ(document.size(), 2U);
        EXPECT_TRUE(document["data"].contains("playerBoard"));
        EXPECT_FALSE(document["data"].contains("name"));
    }

    TEST(SaveArchiveTests, TheMetaBlockCarriesTheUuidTheMomentAndTheName) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};

        archive.save("a-save", "my game", makeSnapshot());

        const nlohmann::json meta = nlohmann::json::parse(*storage.read("a-save"))["meta"];
        EXPECT_EQ(meta.size(), 3U);
        EXPECT_EQ(meta["uuid"], "a-save");
        EXPECT_EQ(meta["name"], "my game");
        const auto timestamp = meta["timestamp"].get<std::string>();
        EXPECT_EQ(timestamp, SaveArchive::timestampForNow());
        EXPECT_NE(SaveArchive::momentOf(timestamp), timestamp);
    }

    TEST(SaveArchiveTests, RemembersWhatThePlayerCalledASave) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        archive.save("a-save", "my game", makeSnapshot());

        EXPECT_EQ(archive.nameOf("a-save"), "my game");
    }

    TEST(SaveArchiveTests, HasNoNameForASaveThatIsNotThere) {
        MemorySaveStorage storage;
        const SaveArchive archive{storage};

        EXPECT_EQ(archive.nameOf("missing"), std::nullopt);
    }

    TEST(SaveArchiveTests, HasNoNameForSomethingThatIsNotASave) {
        MemorySaveStorage storage;
        storage.write("broken", "this is not json at all");
        const SaveArchive archive{storage};

        EXPECT_EQ(archive.nameOf("broken"), std::nullopt);
    }

    TEST(SaveArchiveTests, HasNoNameForASaveWithoutAMetaBlock) {
        MemorySaveStorage storage;
        storage.write("older-shape", "{\"settings\": {}}");
        const SaveArchive archive{storage};

        EXPECT_EQ(archive.nameOf("older-shape"), std::nullopt);
    }

    TEST(SaveArchiveTests, ListsSavesNewestFirst) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        saveMadeAt(storage, archive, "one", "oldest", "2026-01-01T09:00:00");
        saveMadeAt(storage, archive, "two", "newest", "2026-09-28T12:00:00");
        saveMadeAt(storage, archive, "three", "middle", "2026-06-15T10:00:00");

        const std::vector<SaveSummary> saves = archive.listSaves();

        ASSERT_EQ(saves.size(), 3U);
        EXPECT_EQ(saves[0].name, "newest");
        EXPECT_EQ(saves[1].name, "middle");
        EXPECT_EQ(saves[2].name, "oldest");
    }

    TEST(SaveArchiveTests, ASummaryCarriesTheMomentItWasMade) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        saveMadeAt(storage, archive, "a-save", "my game", "2026-09-28T14:30:12");

        const std::vector<SaveSummary> saves = archive.listSaves();

        ASSERT_EQ(saves.size(), 1U);
        EXPECT_EQ(saves.front().id, "a-save");
        EXPECT_EQ(saves.front().name, "my game");
        EXPECT_EQ(saves.front().timestamp, "2026-09-28T14:30:12");
    }

    TEST(SaveArchiveTests, SortsTwoSavesOfTheSameMomentByTheirIdentifiers) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        saveMadeAt(storage, archive, "second", "b", "2026-09-28T12:00:00");
        saveMadeAt(storage, archive, "first", "a", "2026-09-28T12:00:00");

        const std::vector<SaveSummary> saves = archive.listSaves();

        ASSERT_EQ(saves.size(), 2U);
        EXPECT_EQ(saves[0].id, "first");
        EXPECT_EQ(saves[1].id, "second");
    }

    TEST(SaveArchiveTests, StillListsASaveItCannotReadSoItCanBeThrownAway) {
        MemorySaveStorage storage;
        storage.write("broken", "{ not json");
        const SaveArchive archive{storage};

        const std::vector<SaveSummary> saves = archive.listSaves();

        ASSERT_EQ(saves.size(), 1U);
        EXPECT_EQ(saves.front().id, "broken");
        EXPECT_EQ(saves.front().name, "broken");
        EXPECT_TRUE(saves.front().timestamp.empty());
    }

    TEST(SaveArchiveTests, SavingUnderTheSameIdReplacesTheSave) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        archive.save("a-save", "first try", makeSnapshot(1));

        archive.save("a-save", "second try", makeSnapshot(7));

        EXPECT_EQ(archive.listSaves().size(), 1U);
        EXPECT_EQ(archive.nameOf("a-save"), "second try");
        EXPECT_EQ(archive.load("a-save")->roundNumber(), 7);
    }

    TEST(SaveArchiveTests, SavingAgainMovesTheSaveToTheMomentItWasRewritten) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        saveMadeAt(storage, archive, "a-save", "my game", "2020-01-01T00:00:00");

        archive.save("a-save", "my game", makeSnapshot());

        EXPECT_GT(archive.listSaves().front().timestamp, std::string{"2020-01-01T00:00:00"});
    }

    TEST(SaveArchiveTests, LoadingWhatIsNotThereGivesNothing) {
        MemorySaveStorage storage;
        const SaveArchive archive{storage};

        EXPECT_EQ(archive.load("missing"), std::nullopt);
    }

    TEST(SaveArchiveTests, LoadingSomethingUnreadableGivesNothing) {
        MemorySaveStorage storage;
        storage.write("broken", "{ not json");
        const SaveArchive archive{storage};

        EXPECT_EQ(archive.load("broken"), std::nullopt);
    }

    TEST(SaveArchiveTests, LoadingJsonWithoutADataBlockGivesNothing) {
        MemorySaveStorage storage;
        storage.write("wrong-shape", "{\"meta\": {\"name\": \"a save\"}}");
        const SaveArchive archive{storage};

        EXPECT_EQ(archive.load("wrong-shape"), std::nullopt);
    }

    TEST(SaveArchiveTests, LoadingASaveOfTheOlderFlatShapeGivesNothing) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        archive.save("a-save", "my game", makeSnapshot());
        const nlohmann::json document = nlohmann::json::parse(*storage.read("a-save"));
        storage.write("flat", document["data"].dump());

        EXPECT_EQ(archive.load("flat"), std::nullopt);
    }

    TEST(SaveArchiveTests, ReportsAFailureToWrite) {
        UnwritableStorage storage;
        SaveArchive archive{storage};

        EXPECT_FALSE(archive.save("a-save", "my game", makeSnapshot()));
    }

    TEST(SaveArchiveTests, RemovesASaveThatIsThere) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        archive.save("a-save", "my game", makeSnapshot());

        EXPECT_TRUE(archive.remove("a-save"));

        EXPECT_TRUE(archive.listSaves().empty());
    }

    TEST(SaveArchiveTests, RemovingWhatIsNotThereReportsSo) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};

        EXPECT_FALSE(archive.remove("missing"));
    }

    TEST(SaveArchiveTests, EveryNewIdentifierIsAUuidOfItsOwn) {
        std::set<std::string> seen;
        for (int made = 0; made < 200; ++made) {
            const std::string id = SaveArchive::newSaveId();

            EXPECT_EQ(id.size(), 36U);
            EXPECT_EQ(id[8], '-');
            EXPECT_EQ(id[13], '-');
            EXPECT_EQ(id[14], '4');
            EXPECT_EQ(id[18], '-');
            EXPECT_EQ(id[23], '-');
            seen.insert(id);
        }

        EXPECT_EQ(seen.size(), 200U);
    }

    TEST(SaveArchiveTests, TwoSavesMadeInTheSameSecondAreStillTwoSaves) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};

        archive.save(SaveArchive::newSaveId(), "first", makeSnapshot());
        archive.save(SaveArchive::newSaveId(), "second", makeSnapshot());

        EXPECT_EQ(archive.listSaves().size(), 2U);
    }

    TEST(SaveArchiveTests, ATimestampForNowSortsAndReadsAsItsMoment) {
        const std::string timestamp = SaveArchive::timestampForNow();

        EXPECT_EQ(timestamp.size(), 19U);
        EXPECT_EQ(timestamp[10], 'T');
        EXPECT_NE(SaveArchive::momentOf(timestamp), timestamp);
    }

    TEST(SaveArchiveTests, TwoTimestampsSortInTheOrderTheyWereMade) {
        EXPECT_LT(std::string{"2026-01-01T09:00:00"}, std::string{"2026-09-28T12:00:00"});
        EXPECT_LT(std::string{"2026-09-28T11:59:59"}, std::string{"2026-09-28T12:00:00"});
    }

    TEST(SaveArchiveTests, SpellsOutTheMomentATimestampStandsFor) {
        EXPECT_EQ(SaveArchive::momentOf("2026-09-28T14:30:12"), "2026-09-28  14:30:12");
    }

    TEST(SaveArchiveTests, LeavesATimestampItCannotReadAlone) {
        EXPECT_EQ(SaveArchive::momentOf("not-a-moment"), "not-a-moment");
        EXPECT_EQ(SaveArchive::momentOf(""), "");
        EXPECT_EQ(SaveArchive::momentOf("2026-09-28 14:30:12"), "2026-09-28 14:30:12");
    }

    TEST(SaveArchiveTests, ASavedMatchComesBackWholeThroughStorage) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        const MatchSnapshot original = makeSnapshot(4);
        archive.save("a-save", "my game", original);

        const std::optional<MatchSnapshot> loaded = archive.load("a-save");

        ASSERT_TRUE(loaded.has_value());
        EXPECT_EQ(loaded->settings().boardSize(), original.settings().boardSize());
        EXPECT_EQ(loaded->bankedSkills(), original.bankedSkills());
        EXPECT_EQ(loaded->playerBoard().ships().size(), original.playerBoard().ships().size());
        EXPECT_EQ(loaded->currentTurn(), original.currentTurn());
    }

    TEST(SaveArchiveTests, TheNameIsStoredBesideTheMatchWithoutDisturbingIt) {
        MemorySaveStorage storage;
        SaveArchive archive{storage};
        archive.save("a-save", "a name with spaces", makeSnapshot(2));

        EXPECT_EQ(archive.nameOf("a-save"), "a name with spaces");
        EXPECT_EQ(archive.load("a-save")->roundNumber(), 2);
    }
}  // namespace cpp_warships::persistence
