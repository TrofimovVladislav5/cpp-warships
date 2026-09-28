#include <application/persistence/FilesystemSaveStorage.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

namespace cpp_warships::persistence {
    namespace {
        /** @brief Extension every save file carries. */
        constexpr const char* SAVE_EXTENSION = ".json";

        /** @brief Whether @p name is one plain file name, and so cannot lead anywhere but
         * into the save directory. Anything with a separator, a parent step or a root could
         * name a file elsewhere, so nothing of the sort is allowed to become a path. */
        [[nodiscard]] bool isPlainFileName(const std::string& name) {
            if (name.empty() || name == "." || name == "..") {
                return false;
            }

            const std::filesystem::path asPath{name};

            return !asPath.has_root_name() && !asPath.has_root_directory() &&
                   ++asPath.begin() == asPath.end();
        }
    }  // namespace

    FilesystemSaveStorage::FilesystemSaveStorage(std::string directoryPath)
        : directoryPath_(std::move(directoryPath)) {}

    FilesystemSaveStorage FilesystemSaveStorage::inCurrentDirectory() {
        return FilesystemSaveStorage{std::filesystem::current_path().string()};
    }

    std::string FilesystemSaveStorage::pathFor(const std::string& name) const {
        if (!isPlainFileName(name)) {
            return {};
        }

        return (std::filesystem::path{directoryPath_} / (name + SAVE_EXTENSION)).string();
    }

    std::vector<std::string> FilesystemSaveStorage::list() const {
        std::vector<std::string> names;

        std::error_code error;
        if (!std::filesystem::is_directory(directoryPath_, error)) {
            return names;
        }

        for (const auto& entry : std::filesystem::directory_iterator{directoryPath_, error}) {
            if (entry.is_regular_file() && entry.path().extension() == SAVE_EXTENSION) {
                names.push_back(entry.path().stem().string());
            }
        }

        return names;
    }

    std::optional<std::string> FilesystemSaveStorage::read(const std::string& name) const {
        const std::string path = pathFor(name);
        if (path.empty()) {
            return std::nullopt;
        }

        std::ifstream file{path};
        std::optional<std::string> contents;

        if (file.is_open()) {
            std::ostringstream buffer;
            buffer << file.rdbuf();
            contents = buffer.str();
        }

        return contents;
    }

    bool FilesystemSaveStorage::write(const std::string& name, const std::string& contents) {
        const std::string path = pathFor(name);
        if (path.empty()) {
            return false;
        }

        std::error_code error;
        std::filesystem::create_directories(directoryPath_, error);

        std::ofstream file{path};
        bool isWritten = false;

        if (file.is_open()) {
            file << contents;
            isWritten = file.good();
        }

        return isWritten;
    }

    bool FilesystemSaveStorage::contains(const std::string& name) const {
        const std::string path = pathFor(name);
        if (path.empty()) {
            return false;
        }

        std::error_code error;
        return std::filesystem::is_regular_file(path, error);
    }

    const std::string& FilesystemSaveStorage::directoryPath() const noexcept {
        return directoryPath_;
    }

    bool FilesystemSaveStorage::remove(const std::string& name) {
        const std::string path = pathFor(name);
        if (path.empty()) {
            return false;
        }

        std::error_code error;
        return std::filesystem::remove(path, error) && !error;
    }
}  // namespace cpp_warships::persistence
