#include "GameSaveCreator.h"

#include <iostream>
#include <filesystem>

#include "SerializerManager.h"
#include "SerializerSkillManager.h"
#include "ViewHelper.h"
#include "save/SerializerFactory.h"
#include "save/SerializerGameField.h"
#include "save/SerializerMatchSettings.h"
#include "save/SerializerSimplifyObjects.h"

GameSaveCreator::GameSaveCreator(GameStateDTO* dto)
    : dto(dto)
    , factory({})
{
    initializeFactories();
}

GameSaveCreator::GameSaveCreator()
    : dto(nullptr)
    , factory({})
{
    initializeFactories();
}

void GameSaveCreator::initializeFactories() {
    factory["manager"] = new SpecificSerializerFactory<SerializerManager>();
    factory["gameField"] = new SpecificSerializerFactory<SerializerGameField>();
    factory["settings"] = new SpecificSerializerFactory<SerializerMatchSettings>();
    factory["skillManager"] = new SpecificSerializerFactory<SerializerSkillManager>();
    factory["simplifyObjects"] = new SpecificSerializerFactory<SerializerSimplifyObjects>();
}

GameSaveCreator::~GameSaveCreator() {
    for (auto& pair : factory) {
        delete pair.second;
    }
}


std::size_t GameSaveCreator::calculateHash(const std::string& data) {
    return std::hash<std::string>{}(data);
}

void GameSaveCreator::xorEncryptDecrypt(std::string& data, const std::string& key) {
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= key[i % key.size()];
    }
}

std::string GameSaveCreator::encryptChecksum(std::size_t checksum, const std::string& key) {
    std::string checksumStr = std::to_string(checksum);
    xorEncryptDecrypt(checksumStr, key);
    return checksumStr;
}

std::size_t GameSaveCreator::decryptChecksum(const std::string& encryptedChecksum, const std::string& key) {
    std::string decryptedChecksum = encryptedChecksum;
    xorEncryptDecrypt(decryptedChecksum, key);
    return std::stoull(decryptedChecksum);
}

json GameSaveCreator::distributorCreateSave() {
    json baseJson = {
        {"SimplifyObjects", factory["simplifyObjects"]->createSerializer()->serialize(*dto)},
        {"Settings", dto->settings ? factory["settings"]->createSerializer()->serialize(*dto) : json()},
        {"SubState", dto->lastSubState}
    };

    if (dto->lastSubState == "InitiateOngoingGameSubState" || dto->lastSubState == "BattleOngoingGameSubState") {
        baseJson["ShipManagers"] = dto->playerManager ? factory["manager"]->createSerializer()->serialize(*dto) : json();
    }
    if (dto->lastSubState == "BattleOngoingGameSubState") {
        baseJson["GameFields"] = dto->playerField ? factory["gameField"]->createSerializer()->serialize(*dto) : json();
        baseJson["SkillManager"] = dto->playerSkillManager ? factory["skillManager"]->createSerializer()->serialize(*dto) : json();
    }
    return baseJson;
}

std::vector<std::string> GameSaveCreator::listSaves(const std::string& path) {
    std::vector<std::string> jsonFiles;

    if (!std::filesystem::is_directory(path)) {
        ViewHelper::errorOut("Error listing saves: path is not a directory");
        return {};
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                jsonFiles.push_back(entry.path().string());
            }
        }

        return jsonFiles;
    } catch (std::exception& e) {
        ViewHelper::errorOut("Error listing saves", e);
        return {};
    }
}

void GameSaveCreator::createSave(const std::string& filename) {
    json j = distributorCreateSave();
    std::string jsonData = j.dump();
    std::size_t checkSum = calculateHash(jsonData);
    std::string encryptedChecksum = encryptChecksum(checkSum, "encryptKey");
    json saveFile {
            {"data", j},
            {"checksum", encryptedChecksum},
    };
    try {
        JsonFileHandler fileHandler(filename, JsonFileHandler::Mode::Write);
        fileHandler << saveFile;
    } catch (const std::runtime_error& e) {
        ViewHelper::errorOut("Error saving file: " + std::string(e.what()));
    }
}


GameStateDTO* GameSaveCreator::loadSave(const std::string& filename) {
    json saveFile;
    try {
        JsonFileHandler fileHandler(filename, JsonFileHandler::Mode::Read);
        fileHandler >> saveFile;
    } catch (const std::runtime_error& e) {
        ViewHelper::errorOut("Error loading save file: " + std::string(e.what()));
        return nullptr;
    }

    if (!saveFile.contains("checksum") || !saveFile["checksum"].is_string()) {
        ViewHelper::errorOut("Error loading save file: checksum is missing");
        return nullptr;
    }

    json data = saveFile.at("data");
    std::string encryptedChecksum = saveFile.at("checksum");
    std::string closedKey = "encryptKey";
    std::size_t decryptedChecksum = decryptChecksum(encryptedChecksum, closedKey);

    std::string jsonData = data.dump();
    std::size_t computedChecksum = calculateHash(jsonData);

    if (computedChecksum != decryptedChecksum) {
        ViewHelper::errorOut("Error loading save file: checksum mismatch");
        return nullptr;
    }

    return distributorLoadSave(data);
}

GameStateDTO* GameSaveCreator::distributorLoadSave(const json &j) {
    GameStateDTO* dto = new GameStateDTO();
    dto->lastSubState = j.at("SubState").get<std::string>();
    std::vector<std::string> order = {
        "SimplifyObjects",
        "Settings",
        "ShipManagers",
    };
    if ( dto->lastSubState == "BattleOngoingGameSubState") {
        order.emplace_back("GameFields");
        order.emplace_back("SkillManager");
    }

    std::unordered_map<std::string, std::function<void(const json&)>> deserializers = {
        {"SimplifyObjects", [dto](const json& simplifyJson) {
            if (simplifyJson.contains("fieldSize") && simplifyJson["fieldSize"].is_number()) {
                dto->fieldSize = simplifyJson["fieldSize"].get<int>();
            }
            if (simplifyJson.contains("roundNumber") && simplifyJson["roundNumber"].is_number()) {
                dto->roundNumber = simplifyJson["roundNumber"].get<int>();
            }
            if (simplifyJson.contains("shipsSizes") && simplifyJson["shipsSizes"].is_object()) {
                for (const auto& [size, count] : simplifyJson["shipsSizes"].items()) {
                    dto->shipsSizes[std::stoi(size)] = count.get<int>();
                }
            }
        }},
        {"Settings", [this, dto](const json& settingsJson) {
            factory["settings"]->createSerializer()->deserialize(settingsJson, *dto);
        }},
        {"ShipManagers", [this, dto](const json& shipManagersJson) {
            factory["manager"]->createSerializer()->deserialize(shipManagersJson, *dto);
        }},
        {"GameFields", [this, dto](const json& gameFieldsJson) {
            factory["gameField"]->createSerializer()->deserialize(gameFieldsJson, *dto);
        }},
       {"SkillManager", [this, dto](const json& skillManagerJson) {
           factory["skillManager"]->createSerializer()->deserialize(skillManagerJson, *dto);
       }},
   };

    for (const auto& key : order) {
        if (j.contains(key) && j[key].is_object()) {
            if (deserializers[key]) {
                deserializers[key](j[key]);
            }
        }
    }

    if (dto->lastSubState == "BattleOngoingGameSubState" && !dto->playerSkillManager) {
        dto->playerSkillManager = new SkillManager({},dto->enemyField, dto->settings);
    }

    return dto;
}