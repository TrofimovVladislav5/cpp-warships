#include "GameSaveCreator.h"

#include <iostream>

#include "SerializerManager.h"
#include "SerializerGameField.h"
#include "SerializerMatchSettings.h"
#include "SerializerSimplifyObjects.h"
#include "SerializerSkillManager.h"
#include "SerializerFactory.h"
#include "ViewHelper.h"

GameSaveCreator::GameSaveCreator(GameStateDTO* dto)
    : dto(dto)
{
    initializeFactories();
}

GameSaveCreator::GameSaveCreator()
    : dto(nullptr)
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

void GameSaveCreator::createSave(const std::string& filename) {
    json j = json {
            {"ShipManagers", dto->playerManager ? factory["manager"]->createSerializer()->serialize(*dto) : json()},
            {"GameFields", dto->playerField ? factory["gameField"]->createSerializer()->serialize(*dto) : json()},
            {"Settings", dto->settings ? factory["settings"]->createSerializer()->serialize(*dto) : json()},
            {"SkillManager", dto->playerSkillManager ? factory["skillManager"]->createSerializer()->serialize(*dto) : json()},
            {"SimplifyObjects", factory["simplifyObjects"]->createSerializer()->serialize(*dto)},
                {"SubState", dto->lastSubState}
    };
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
    }catch (const std::runtime_error& e) {
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

    GameStateDTO* dto = new GameStateDTO();

    std::vector<std::string> deserializationOrder = getDeserializationOrder(data);
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
        {"Settings", [this, dto](const json& j) {
            factory["settings"]->createSerializer()->deserialize(j, *dto);
        }},
        {"ShipManagers", [this, dto](const json& j) {
            factory["manager"]->createSerializer()->deserialize(j, *dto);
        }},
        {"GameFields", [this, dto](const json& j) {
            factory["gameField"]->createSerializer()->deserialize(j, *dto);
        }},
        {"SkillManager", [this, dto](const json& j) {
            factory["skillManager"]->createSerializer()->deserialize(j, *dto);
        }}
    };

    if (data.contains("SubState") && data["SubState"].is_string()) {
        dto->lastSubState = data["SubState"].get<std::string>();
    }

    for (const auto& key : deserializationOrder) {
        if (data.contains(key) && data[key].is_object()) {
            deserializers[key](data[key]);
        }
    }

    return dto;
}

std::vector<std::string> GameSaveCreator::getDeserializationOrder(const json &j) const {
    std::vector<std::string> order = {};
    std::string subState = j.at("SubState").get<std::string>();
    if (subState == "InitiateOngoingGameSubState") {
        order = {
            "SimplifyObjects",
            "Settings"
            "ShipManagers",
        };
    }
    else if (subState== "BattleOngoingGameSubState") {
        order = {
            "SimplifyObjects",
            "Settings",
            "ShipManagers",
            "GameFields",
            "SkillManager"
        };
    }
    return order;
}