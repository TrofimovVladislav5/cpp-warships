#pragma once

#include <application/persistence/MatchSnapshot.h>
#include <application/persistence/SaveSummary.h>

#include <optional>
#include <string>
#include <vector>

namespace cpp_warships::persistence {
    class SaveStorage;
}

namespace cpp_warships::persistence {
    /** @brief Saves and loads matches, turning snapshots into stored text and back. */
    class SaveArchive {
    public:
        explicit SaveArchive(SaveStorage& storage);

        /** @brief Every save there is, newest first. */
        [[nodiscard]] std::vector<SaveSummary> listSaves() const;

        /** @brief An id for a save made now, which sorts and reads as its moment. */
        [[nodiscard]] static std::string idForNow();

        /** @brief The moment @p id stands for, written out for a player to read. */
        [[nodiscard]] static std::string momentOf(const std::string& id);

        /** @brief Throws the save called @p name away. @return whether one was there. */
        bool remove(const std::string& id);

        /** @brief Writes @p snapshot into the save called @p id, under the name the
         * player gave it. @return false when it could not be stored. */
        bool save(const std::string& id, const std::string& name, const MatchSnapshot& snapshot);

        /** @brief Reads the save called @p id, or nullopt when it is missing or unreadable. */
        [[nodiscard]] std::optional<MatchSnapshot> load(const std::string& id) const;

        /** @brief What the player called the save @p id, without reading the whole match. */
        [[nodiscard]] std::optional<std::string> nameOf(const std::string& id) const;

    private:
        SaveStorage& storage_;
    };
}  // namespace cpp_warships::persistence
