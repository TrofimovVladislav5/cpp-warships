#pragma once
#include "game/GameStateDTO.h"
#include "JsonFileHandler.h"
#include <map>
#include <string>

#include "save/SerializerFactory.h"

class GameSaveCreator {
private:
    GameStateDTO* dto;
    std::map<std::string, SerializerFactory*> factory;
    size_t calculateHash(const std::string& data);
    void xorEncryptDecrypt(std::string& data, const std::string& key);
    [[nodiscard]] std::string encryptChecksum(std::size_t checksum, const std::string& key);
    [[nodiscard]] std::size_t decryptChecksum(const std::string& encryptedChecksum, const std::string& key);
    void initializeFactories();
    GameStateDTO* distributorLoadSave(const json& j);
    json distributorCreateSave();
public:
    explicit GameSaveCreator(GameStateDTO* dto);
    explicit GameSaveCreator();
    ~GameSaveCreator();
    void createSave(const std::string& filename);
    [[nodiscard]] GameStateDTO* loadSave(const std::string& filename);
};

