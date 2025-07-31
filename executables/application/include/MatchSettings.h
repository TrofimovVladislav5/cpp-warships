#pragma once

#include <map>

namespace cpp_warships::application {

    class MatchSettings {
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