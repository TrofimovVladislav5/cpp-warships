#pragma once
#include "../library/parser/Parser.h"
#include "../model/StateContext.h"

class MatchSettingsController {
private:
    std::map<int, int> calculateOptimalLengthShips(int fieldSize);

public:
    explicit MatchSettingsController();
    ~MatchSettingsController();
    MatchSettings* createMatchSettings(int fieldSize);
};