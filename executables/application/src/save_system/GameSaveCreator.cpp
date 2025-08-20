#include "../../include/save_system/GameSaveCreator.h"

#include <cpp_warships/utilities/include/ViewHelper.h>
#include <filesystem>
#include <utility>

#include "../../include/save_system/JsonFileHandler.h"
#include "../../include/save_system/GameFieldJsonSerializer.h"
#include "../../include/save_system/GameStateJsonSerializer.h"

namespace cpp_warships::application {
    GameSaveCreator::GameSaveCreator(GameStateDTO* dto, std::string saveDirectory)
        : saveDirectoryPath(std::move(saveDirectory))
        , referenceDto(dto)
    {
        initiateSerializers();
    }

    GameSaveCreator::GameSaveCreator()
        : saveDirectoryPath(std::filesystem::current_path())
        , referenceDto(nullptr)
    {
        initiateSerializers();
    }

    GameSaveCreator::GameSaveCreator(std::string  saveDirectory)
        : saveDirectoryPath(std::move(saveDirectory))
        , referenceDto(nullptr)
    {
        initiateSerializers();
    }

    GameSaveCreator::GameSaveCreator(GameStateDTO* dto)
        : saveDirectoryPath(std::filesystem::current_path())
        , referenceDto(dto)
    {
        initiateSerializers();
    }

    std::vector<std::string> GameSaveCreator::getJsonSaves() const {
        std::vector<std::string> jsonFiles;
        for (const auto& entry : std::filesystem::directory_iterator(saveDirectoryPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                jsonFiles.push_back(entry.path().string());
            }
        }

        return jsonFiles;
    }

    std::vector<std::string> GameSaveCreator::listSaves() const {
        if (!std::filesystem::is_directory(saveDirectoryPath)) {
            ViewHelper::errorOut("Error listing saves: path is not a directory");
            return {};
        }

        try {
            return getJsonSaves();
        } catch (std::exception& e) {
            ViewHelper::errorOut("Error listing saves", e);
            return {};
        }
    }

    bool GameSaveCreator::createSave(const std::string& filename) {
        bool isSuccessful = false;

        try {
            nlohmann::json serializedData = serializer.serialize(*referenceDto);
            std::string finalPath = saveDirectoryPath + "/" + filename;
            JsonFileHandler fileHandler(finalPath, JsonFileHandler::Mode::Write);

            fileHandler << serializedData;
            isSuccessful = true;
        } catch (const std::runtime_error& e) {
            ViewHelper::errorOut("Error saving to file: " + std::string(e.what()));
        }

        return isSuccessful;
    }

    GameStateDTO* GameSaveCreator::loadSave(const std::string& filename) {
        try {
            nlohmann::json saveFile;
            JsonFileHandler fileHandler(filename, JsonFileHandler::Mode::Read);
            fileHandler >> saveFile;

            auto resultObject = serializer.deserialize<GameStateDTO>(saveFile);
            auto* referenceObject = new GameStateDTO(resultObject);
            return referenceObject;
        } catch (const std::runtime_error& e) {
            ViewHelper::errorOut("Error loading save file: " + std::string(e.what()));
            return nullptr;
        }
    }

    void GameSaveCreator::initiateSerializers() {
        serializer = game_saves::SerializerAggregator<nlohmann::json>();

        serializer.setSerializers(
            new GameFieldJsonSerializer{},
            new SkillManagerJsonSerializer{},
            new GameStateJsonSerializer{},
            new SegmentJsonSerializer{},
            new ShipJsonSerializer{},
            new ShipManagerJsonSerializer{}
        );
    }
} // namespace cpp_warships::application