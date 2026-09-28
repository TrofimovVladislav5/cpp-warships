#pragma once

#include <application/persistence/MatchSnapshot.h>

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

        [[nodiscard]] std::vector<std::string> listSaves() const;

        /** @brief Writes @p snapshot under @p name. @return false when it could
         * not be stored. */
        bool save(const std::string& name, const MatchSnapshot& snapshot);

        /** @brief Reads the save called @p name, or nullopt when it is missing
         * or unreadable. */
        [[nodiscard]] std::optional<MatchSnapshot> load(const std::string& name) const;

    private:
        SaveStorage& storage_;
    };
}  // namespace cpp_warships::persistence
