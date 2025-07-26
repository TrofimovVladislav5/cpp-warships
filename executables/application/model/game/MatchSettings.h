#pragma once

#include <map>

namespace cpp_warships::application {

    class MatchSettings {
    private:
        std::map<int, int> shipSizesDictionary;
        int fieldSize;

    public:
        explicit MatchSettings(std::map<int, int> shipSizesDictionary, int fieldSize);
        int damageCount;
        bool isActiveDoubleDamage = false;
        [[nodiscard]] int getFieldSize() const;
        [[nodiscard]] std::map<int, int> getShipsCount() const;
    };
} // namespace cpp_warships::application