#pragma once

#include <cpp_warships/game_saves/include/SerializerAggregator.h>
#include <nlohmann/json.hpp>
#include <string>

#include "../GameStateDTO.h"

namespace cpp_warships::application {

    class GameSaveCreator {
    private:
        const std::string saveDirectoryPath;
        GameStateDTO* referenceDto;
        game_saves::SerializerAggregator<nlohmann::json> serializer;

        [[nodiscard]] std::vector<std::string> getJsonSaves() const;
        void initiateSerializers();
    public:
        explicit GameSaveCreator(std::string saveDirectory);
        explicit GameSaveCreator(GameStateDTO* dto);
        GameSaveCreator(GameStateDTO* dto, std::string saveDirectory);
        GameSaveCreator();
        ~GameSaveCreator() = default;

        [[nodiscard]] std::vector<std::string> listSaves() const;
        bool createSave(const std::string& filename);
        [[nodiscard]] GameStateDTO* loadSave(const std::string& filename);
    };
} // namespace cpp_warships::application
