#pragma once
#include "game/GameStateDTO.h"
#include "SerializerFactory.h"
#include "JsonFileHandler.h"
#include <map>
#include <string>

class GameSaveCreator {
private:
    GameStateDTO dto;
    std::map<std::string, SerializerFactory*> factory;
    size_t calculateHash(const std::string& data);
    void xorEncryptDecrypt(std::string& data, const std::string& key);
    [[nodiscard]] std::string encryptChecksum(std::size_t checksum, const std::string& key);
    [[nodiscard]] std::size_t decryptChecksum(const std::string& encryptedChecksum, const std::string& key);
    void initializeFactories();
public:
    explicit GameSaveCreator(const GameStateDTO& dto);
    explicit GameSaveCreator();
    ~GameSaveCreator();
    void createSave();
    [[nodiscard]] GameStateDTO loadSave();
};

