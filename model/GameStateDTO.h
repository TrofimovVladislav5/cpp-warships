#pragma once
#include "MatchSettings.h"

class GameStateDTO {
private:
    MatchSettings* settings;
public:
    GameStateDTO();
    explicit GameStateDTO(MatchSettings* settings);
    MatchSettings* getSettings();
    void setSettings(MatchSettings* newSettings);
    int roundNumber;
    int currentShips;
};