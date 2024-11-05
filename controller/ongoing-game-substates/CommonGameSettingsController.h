#pragma once
#include "StateContext.h"
#include "../library/parser/Parser.h"
#include <vector>
#include <string>

class CommonGameSettingsController {
private:
    MatchSettings* settings;
    StateContext context;
    bool isFieldSizeSet = false;
    bool settingsIsSet = false;
    void calculateOptimalLengthShips(int fieldSize, std::vector<int>& shipsCounts);
public:
    explicit CommonGameSettingsController(StateContext& context);
    ~CommonGameSettingsController();
    void handleGameFieldSize(ParsedOptions options);
    bool isFinishedSettings() const;
};