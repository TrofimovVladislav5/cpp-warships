#pragma once
#include "StateContext.h"
#include "../library/parser/Parser.h"
#include <vector>
#include <string>

class CommonGameSettingsController {
private:
    MatchSettings* settings;
    StateContext context;
    void getVectorFromString(std::string str, std::vector<int>& potentialShipsLength);
    void calculateOptimalLengthShips(int fieldSize, std::vector<int>& shipsCounts);
    bool isFieldSizeSet = false;
    bool settingsIsSet = false;
public:
    CommonGameSettingsController(StateContext& context);
    ~CommonGameSettingsController();
    void handleGameFieldSize(ParsedOptions options);
    bool isFinishedSettings() const;
};