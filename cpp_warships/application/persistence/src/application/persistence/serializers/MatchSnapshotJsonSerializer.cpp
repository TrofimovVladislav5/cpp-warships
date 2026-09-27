#include <application/persistence/serializers/MatchSnapshotJsonSerializer.h>

#include <serialization/exceptions/DeserializationException.h>

#include <algorithm>
#include <deque>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace cpp_warships::persistence {
    namespace {
        const std::unordered_map<flow::SkillKind, std::string> NAME_BY_SKILL{
                {flow::SkillKind::Scanner, "scanner"},
                {flow::SkillKind::DoubleDamage, "doubleDamage"},
                {flow::SkillKind::RandomStrike, "randomStrike"}
        };

        const std::unordered_map<flow::MatchPhase, std::string> NAME_BY_PHASE{
                {flow::MatchPhase::Placement, "placement"},
                {flow::MatchPhase::Battle, "battle"},
                {flow::MatchPhase::Finished, "finished"}
        };

        const std::unordered_map<flow::Participant, std::string> NAME_BY_PARTICIPANT{
                {flow::Participant::Player, "player"},
                {flow::Participant::Computer, "computer"}
        };

        template <typename TValue>
        TValue valueForName(
                const std::unordered_map<TValue, std::string>& namesByValue,
                const std::string& name,
                const std::string& what
        ) {
            const auto matchesName = [&name](const std::pair<const TValue, std::string>& entry) {
                return entry.second == name;
            };

            const auto found = std::find_if(namesByValue.begin(), namesByValue.end(), matchesName);
            if (found == namesByValue.end()) {
                throw serialization::exceptions::DeserializationException(
                        what,
                        "unknown name: " + name
                );
            }

            return found->first;
        }
        [[nodiscard]] nlohmann::json coordinatesToJson(
                const std::vector<core::Coordinate>& coordinates
        ) {
            nlohmann::json written = nlohmann::json::array();
            for (const core::Coordinate& coordinate : coordinates) {
                written.push_back({{"x", coordinate.x}, {"y", coordinate.y}});
            }

            return written;
        }

        [[nodiscard]] std::vector<core::Coordinate> coordinatesFromJson(
                const nlohmann::json& written
        ) {
            std::vector<core::Coordinate> coordinates;
            for (const nlohmann::json& coordinate : written) {
                coordinates.push_back({coordinate["x"].get<int>(), coordinate["y"].get<int>()});
            }

            return coordinates;
        }

        /** @brief What the computer knows, written out so a loaded game keeps a sharp enemy. */
        [[nodiscard]] nlohmann::json memoryToJson(const flow::AiMemory& memory) {
            const std::vector<core::Coordinate> attempted{
                    memory.attemptedCoordinates.begin(),
                    memory.attemptedCoordinates.end()
            };

            return nlohmann::json{
                    {"attemptedCoordinates", coordinatesToJson(attempted)},
                    {"currentTargetHits", coordinatesToJson(memory.currentTargetHits)}
            };
        }

        /** @brief The computer's knowledge read back, empty for a save written before it
         *  was kept, which simply means that enemy starts the load looking again. */
        [[nodiscard]] flow::AiMemory memoryFromJson(const nlohmann::json& item) {
            if (!item.contains("opponentMemory")) {
                return {};
            }

            const nlohmann::json& memory = item["opponentMemory"];
            const std::vector<core::Coordinate> attempted =
                    coordinatesFromJson(memory["attemptedCoordinates"]);

            return flow::AiMemory{
                    .attemptedCoordinates = {attempted.begin(), attempted.end()},
                    .currentTargetHits = coordinatesFromJson(memory["currentTargetHits"])
            };
        }
    } // namespace

    bool MatchSnapshotJsonSerializer::isRelated(nlohmann::json item) {
        return item.contains("settings") && item.contains("playerBoard") &&
               item.contains("computerBoard") && item.contains("phase");
    }

    nlohmann::json MatchSnapshotJsonSerializer::serialize(MatchSnapshot& item) {
        BoardJsonSerializer boardSerializer = std::get<0>(childrenSerializers);
        MatchSettingsJsonSerializer settingsSerializer = std::get<1>(childrenSerializers);

        core::Board playerBoard = item.playerBoard();
        core::Board computerBoard = item.computerBoard();
        core::MatchSettings settings = item.settings();

        nlohmann::json bankedSkills = nlohmann::json::array();
        for (const flow::SkillKind skill : item.bankedSkills()) {
            bankedSkills.push_back(NAME_BY_SKILL.at(skill));
        }

        return nlohmann::json{
                {"settings", settingsSerializer.serialize(settings)},
                {"playerBoard", boardSerializer.serialize(playerBoard)},
                {"computerBoard", boardSerializer.serialize(computerBoard)},
                {"bankedSkills", bankedSkills},
                {"roundNumber", item.roundNumber()},
                {"phase", NAME_BY_PHASE.at(item.phase())},
                {"currentTurn", NAME_BY_PARTICIPANT.at(item.currentTurn())},
                {"isDoubleDamageArmed", item.isDoubleDamageArmed()},
                {"opponentMemory", memoryToJson(item.opponentMemory())}
        };
    }

    MatchSnapshot MatchSnapshotJsonSerializer::deserialize(nlohmann::json item) {
        if (!isRelated(item)) {
            throw serialization::exceptions::DeserializationException(
                    "MatchSnapshot",
                    "JSON does not describe a match snapshot"
            );
        }

        BoardJsonSerializer boardSerializer = std::get<0>(childrenSerializers);
        MatchSettingsJsonSerializer settingsSerializer = std::get<1>(childrenSerializers);

        std::deque<flow::SkillKind> bankedSkills;
        for (const nlohmann::json& skill : item["bankedSkills"]) {
            bankedSkills.push_back(
                    valueForName(NAME_BY_SKILL, skill.get<std::string>(), "SkillKind")
            );
        }

        const flow::MatchPhase phase =
                valueForName(NAME_BY_PHASE, item["phase"].get<std::string>(), "MatchPhase");
        const flow::Participant currentTurn = valueForName(
                NAME_BY_PARTICIPANT,
                item["currentTurn"].get<std::string>(),
                "Participant"
        );

        return MatchSnapshot{
                settingsSerializer.deserialize(item["settings"]),
                boardSerializer.deserialize(item["playerBoard"]),
                boardSerializer.deserialize(item["computerBoard"]),
                std::move(bankedSkills),
                item["roundNumber"].get<int>(),
                phase,
                currentTurn,
                item["isDoubleDamageArmed"].get<bool>(),
                memoryFromJson(item)
        };
    }
} // namespace cpp_warships::persistence
