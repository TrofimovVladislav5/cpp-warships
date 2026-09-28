#include <application/persistence/FilesystemSaveStorage.h>
#include <build_save_archive.h>

#include <filesystem>
#include <optional>
#include <platform/OSBrancher.h>
#include <string>
#include <utility>

namespace cpp_warships::application {
    namespace {
        const std::string SAVE_DIRECTORY_NAME = ".cpp-warships";

    }  // namespace

    std::string defaultSaveDirectory() {
        const std::optional<std::string> home = platform::homeDirectory();
        const std::filesystem::path root =
            home.has_value() ? std::filesystem::path{*home} : std::filesystem::current_path();

        return (root / SAVE_DIRECTORY_NAME).string();
    }

    SaveLibrary buildSaveLibrary(const std::string& saveDirectory) {
        auto storage = std::make_unique<persistence::FilesystemSaveStorage>(saveDirectory);
        auto archive = std::make_unique<persistence::SaveArchive>(*storage);

        return SaveLibrary{.storage = std::move(storage), .archive = std::move(archive)};
    }
}  // namespace cpp_warships::application
