#include <application/persistence/SaveArchive.h>
#include <application/persistence/SaveStorage.h>
#include <application/persistence/serializers/MatchSnapshotJsonSerializer.h>

#include <algorithm>
#include <chrono>
#include <ctime>
#include <exception>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <sstream>

namespace cpp_warships::persistence {
    namespace {
        constexpr std::size_t DATE_LENGTH = 8;
        constexpr std::size_t SAVE_NAME_LENGTH = 15;
        constexpr const char* SAVE_NAME_KEY = "name";
    }  // namespace

    namespace {
        /** @brief How many spaces a written save is indented by, to stay
         * readable. */
        constexpr int SAVE_INDENTATION = 4;

        /** @brief A snapshot serializer with its children wired up. */
        serializers::MatchSnapshotJsonSerializer makeSnapshotSerializer() {
            serializers::MatchSnapshotJsonSerializer serializer;
            serializers::BoardJsonSerializer boardSerializer;
            serializers::MatchSettingsJsonSerializer settingsSerializer;
            serializers::ShipJsonSerializer shipSerializer;
            serializers::SegmentJsonSerializer segmentSerializer;

            shipSerializer.setChildrenSerializers(&segmentSerializer);
            boardSerializer.setChildrenSerializers(&shipSerializer);
            serializer.setChildrenSerializers(&boardSerializer, &settingsSerializer);

            return serializer;
        }
    }  // namespace

    SaveArchive::SaveArchive(SaveStorage& storage)
        : storage_(storage) {}

    std::vector<SaveSummary> SaveArchive::listSaves() const {
        std::vector<std::string> ids = storage_.list();
        std::sort(ids.begin(), ids.end(), std::greater<>());

        std::vector<SaveSummary> summaries;
        summaries.reserve(ids.size());
        for (const std::string& id : ids) {
            summaries.push_back({.id = id, .name = nameOf(id).value_or(momentOf(id))});
        }

        return summaries;
    }

    std::optional<std::string> SaveArchive::nameOf(const std::string& id) const {
        const std::optional<std::string> contents = storage_.read(id);
        if (!contents.has_value()) {
            return std::nullopt;
        }

        try {
            const nlohmann::json document = nlohmann::json::parse(*contents);
            if (document.contains(SAVE_NAME_KEY)) {
                return document[SAVE_NAME_KEY].get<std::string>();
            }
        } catch (const std::exception&) {
            return std::nullopt;
        }

        return std::nullopt;
    }

    std::string SaveArchive::idForNow() {
        const std::time_t now =
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm broken{};
        localtime_r(&now, &broken);

        std::ostringstream written;
        written << std::put_time(&broken, "%Y%m%d-%H%M%S");

        return written.str();
    }

    std::string SaveArchive::momentOf(const std::string& id) {
        const std::string& name = id;
        if (name.size() != SAVE_NAME_LENGTH || name[DATE_LENGTH] != '-') {
            return name;
        }

        return name.substr(0, 4) + "-" + name.substr(4, 2) + "-" + name.substr(6, 2) + "  " +
               name.substr(9, 2) + ":" + name.substr(11, 2) + ":" + name.substr(13, 2);
    }

    bool SaveArchive::remove(const std::string& id) {
        return storage_.remove(id);
    }

    bool SaveArchive::save(
        const std::string& id,
        const std::string& name,
        const MatchSnapshot& snapshot
    ) {
        serializers::MatchSnapshotJsonSerializer serializer = makeSnapshotSerializer();
        MatchSnapshot writableSnapshot = snapshot;
        bool isSaved = false;

        try {
            nlohmann::json document = serializer.serialize(writableSnapshot);
            document[SAVE_NAME_KEY] = name;
            isSaved = storage_.write(id, document.dump(SAVE_INDENTATION));
        } catch (const std::exception&) {
            isSaved = false;
        }

        return isSaved;
    }

    std::optional<MatchSnapshot> SaveArchive::load(const std::string& id) const {
        const std::optional<std::string> contents = storage_.read(id);
        std::optional<MatchSnapshot> snapshot;

        if (contents.has_value()) {
            serializers::MatchSnapshotJsonSerializer serializer = makeSnapshotSerializer();
            try {
                snapshot = serializer.deserialize(nlohmann::json::parse(*contents));
            } catch (const std::exception&) {
                snapshot = std::nullopt;
            }
        }

        return snapshot;
    }
}  // namespace cpp_warships::persistence
