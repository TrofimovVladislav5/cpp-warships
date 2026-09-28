#include <application/persistence/SaveArchive.h>
#include <application/persistence/SaveStorage.h>
#include <application/persistence/serializers/MatchSnapshotJsonSerializer.h>

#include <exception>
#include <nlohmann/json.hpp>

namespace cpp_warships::persistence {
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

    std::vector<std::string> SaveArchive::listSaves() const {
        return storage_.list();
    }

    bool SaveArchive::save(const std::string& name, const MatchSnapshot& snapshot) {
        serializers::MatchSnapshotJsonSerializer serializer = makeSnapshotSerializer();
        MatchSnapshot writableSnapshot = snapshot;
        bool isSaved = false;

        try {
            const nlohmann::json document = serializer.serialize(writableSnapshot);
            isSaved = storage_.write(name, document.dump(SAVE_INDENTATION));
        } catch (const std::exception&) {
            isSaved = false;
        }

        return isSaved;
    }

    std::optional<MatchSnapshot> SaveArchive::load(const std::string& name) const {
        const std::optional<std::string> contents = storage_.read(name);
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
