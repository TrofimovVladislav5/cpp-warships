#pragma once
#include "../model/StateContext.h"
#include "../library/parser/Parser.h"


class MatchSettingsController {
private:
    std::map<int, int> calculateOptimalLengthShips(int fieldSize);
public:
    explicit MatchSettingsController();
    ~MatchSettingsController();
    MatchSettings* createMatchSettings(int fieldSize);
};