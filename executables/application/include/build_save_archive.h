#pragma once

#include <memory>
#include <string>

#include <game_persistence/SaveArchive.h>
#include <game_persistence/SaveStorage.h>

namespace cpp_warships::application {
    /** @brief Where a session's saves are kept, and the archive that reads and writes them.
     *  Held together because the archive borrows the storage and must not outlive it. */
    struct SaveLibrary {
        std::unique_ptr<game_persistence::SaveStorage> storage;
        std::unique_ptr<game_persistence::SaveArchive> archive;
    };

    /** @brief Saves kept as files under @p saveDirectory.
     *  This is the one place that settles where a session's saves live. */
    [[nodiscard]] SaveLibrary buildSaveLibrary(const std::string& saveDirectory);

    /** @brief The directory saves go in unless the player says otherwise. */
    [[nodiscard]] std::string defaultSaveDirectory();
} // namespace cpp_warships::application
