#pragma once

#include <application/persistence/SaveArchive.h>
#include <application/persistence/SaveStorage.h>

#include <memory>
#include <string>

namespace cpp_warships::application {
    /** @brief Where a session's saves are kept, and the archive that reads and writes them. */
    struct SaveLibrary {
        std::unique_ptr<persistence::SaveStorage> storage;
        std::unique_ptr<persistence::SaveArchive> archive;
    };

    /** @brief Saves kept as files under @p saveDirectory.
     * This is the one place that settles where a session's saves live. */
    [[nodiscard]] SaveLibrary buildSaveLibrary(const std::string& saveDirectory);

    /** @brief The directory saves go in unless the player says otherwise. */
    [[nodiscard]] std::string defaultSaveDirectory();
}  // namespace cpp_warships::application
