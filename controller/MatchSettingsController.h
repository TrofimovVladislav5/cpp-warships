#pragma once
#include "../model/StateContext.h"
#include "../library/parser/Parser.h"
#include <vector>

class MatchSettingsController {
private:
    std::vector<int> calculateOptimalLengthShips(int fieldSize);
public:
    explicit MatchSettingsController();
    ~MatchSettingsController();
    MatchSettings* createMatchSettings(int fieldSize);
};