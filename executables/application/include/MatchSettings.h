#pragma once

#include <cpp_warships/game_saves/include/ISerializable.h>
#include <map>

namespace cpp_warships::application {

    inline char MatchSettingsName[] = "MatchSettings";

    class MatchSettings
        : public game_saves::ISerializable<MatchSettingsName>
    {
    private:
        std::map<int, int> shipSizesDictionary;
        int fieldSize;
        int damageCount;
        bool isActiveDoubleDamage = false;

    public:
        explicit MatchSettings(std::map<int, int> shipSizesDictionary, int fieldSize);
        void toggleDoubleDamage(bool isActive);
        [[nodiscard]] bool getIsDoubleDamage() const;
        [[nodiscard]] int getDamageCount() const;
        [[nodiscard]] int getFieldSize() const;
        [[nodiscard]] std::map<int, int> getShipsCount() const;
    };
} // namespace cpp_warships::application