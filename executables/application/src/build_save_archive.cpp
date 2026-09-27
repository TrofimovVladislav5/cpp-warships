#include <build_save_archive.h>

#include <cstdlib>
#include <filesystem>

#include <game_persistence/FilesystemSaveStorage.h>

namespace cpp_warships::application {
    namespace {
        const std::string SAVE_DIRECTORY_NAME = ".cpp-warships";
    } // namespace

    std::string defaultSaveDirectory() {
        const char* home = std::getenv("HOME");
        const std::filesystem::path root =
                home == nullptr ? std::filesystem::current_path() : std::filesystem::path{home};

        return (root / SAVE_DIRECTORY_NAME).string();
    }

    SaveLibrary buildSaveLibrary(const std::string& saveDirectory) {
        auto storage = std::make_unique<game_persistence::FilesystemSaveStorage>(saveDirectory);
        auto archive = std::make_unique<game_persistence::SaveArchive>(*storage);

        return SaveLibrary{.storage = std::move(storage), .archive = std::move(archive)};
    }
} // namespace cpp_warships::application
