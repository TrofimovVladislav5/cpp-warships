#include "GameSaveCreator.h"
#include "SerializerManager.h"
#include "SerializerGameField.h"
#include "SerializerMatchSettings.h"
#include "SerializerSimplifyObjects.h"
#include "SerializerSkillManager.h"
#include "SerializerFactory.h"
#include "ViewHelper.h"

GameSaveCreator::GameSaveCreator(const GameStateDTO& dto)
    : dto(dto)
{
    initializeFactories();
}

GameSaveCreator::GameSaveCreator()
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

void GameSaveCreator::createSave() {
    json j = json {
            {"ShipManagers", dto.playerManager ? factory["manager"]->createSerializer()->serialize(dto) : json()},
            {"GameFields", dto.playerField ? factory["gameField"]->createSerializer()->serialize(dto) : json()},
            {"Settings", dto.settings ? factory["settings"]->createSerializer()->serialize(dto) : json()},
            {"SkillManager", dto.playerSkillManager ? factory["skillManager"]->createSerializer()->serialize(dto) : json()},
            {"SimplifyObjects", factory["simplifyObjects"]->createSerializer()->serialize(dto)}
    };
    std::string jsonData = j.dump();
    std::size_t checkSum = calculateHash(jsonData);
    std::string encryptedChecksum = encryptChecksum(checkSum, "encryptKey");

    json saveFile {
            {"data", j},
            {"checksum", encryptedChecksum}
    };

    try {
        JsonFileHandler fileHandler("save.json", JsonFileHandler::Mode::Write);
        fileHandler << saveFile;
    }catch (const std::runtime_error& e) {
        ViewHelper::errorOut("Error saving file: " + std::string(e.what()));
    }
}

GameStateDTO GameSaveCreator::loadSave() {
    json saveFile;
    try {
        JsonFileHandler fileHandler("save.json", JsonFileHandler::Mode::Read);
        fileHandler >> saveFile;
    } catch (const std::runtime_error& e) {
        ViewHelper::errorOut("Error loading save file: " + std::string(e.what()));
        return GameStateDTO();
    }

    if (!saveFile.contains("checksum") || !saveFile["checksum"].is_string()) {
        ViewHelper::errorOut("Error loading save file: checksum is missing");
        return GameStateDTO();
    }

    json data = saveFile.at("data");
    std::string encryptedChecksum = saveFile.at("checksum");
    std::string closedKey = "encryptKey";
    std::size_t decryptedChecksum = decryptChecksum(encryptedChecksum, closedKey);

    std::string jsonData = data.dump();
    std::size_t computedChecksum = calculateHash(jsonData);

    if (computedChecksum != decryptedChecksum) {
        ViewHelper::errorOut("Error loading save file: checksum mismatch");
        return GameStateDTO();
    }

    GameStateDTO dto;
    std::unordered_map<std::string, std::function<void(const json&)> >deserializers = {
            {"ShipManagers", [this, &dto](const json& j) {
                factory["manager"]->createSerializer()->deserialize(j,dto);
            }},
            {"GameFields", [this, &dto](const json& j) {
                factory["gameField"]->createSerializer()->deserialize(j, dto);
            }},
            {"Settings", [this, &dto](const json& j) {
                factory["settings"]->createSerializer()->deserialize(j,dto);
            }},
            {"SkillManager", [this, &dto](const json& j) {
               factory["skillManager"]->createSerializer()->deserialize(j, dto);
            }},
            {"SimplifyObjects", [this, &dto](const json& j) {
                if (j.contains("currentShips") && j["currentShips"].is_number()) {
                    dto.currentShips = j["currentShips"].get<int>();
                }
                if (j.contains("fieldSize") && j["fieldSize"].is_number()) {
                    dto.fieldSize = j["fieldSize"].get<int>();
                }
                if (j.contains("roundNumber") && j["roundNumber"].is_number()) {
                    dto.roundNumber = j["roundNumber"].get<int>();
                }
            }}
    };
    for (const auto& [key, deserializer] : deserializers) {
        if (data.contains(key) && data[key].is_object()) {
            deserializer(data[key]);
        }
    }
    return dto;
}