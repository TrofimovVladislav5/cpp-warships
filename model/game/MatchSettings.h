#pragma once
#include <map>

class MatchSettings {
private:
    std::map<int, int> shipSizesDictionary;
    int fieldSize;
public:
    explicit MatchSettings(std::map<int, int> shipSizesDictionary, int fieldSize);
    ~MatchSettings();
    int damageCount;
    bool isActiveDoubleDamage;
    [[nodiscard]] int getFieldSize() const;
    [[nodiscard]] std::map<int, int> getShipsCount() const;
};
