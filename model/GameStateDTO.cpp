#include "GameStateDTO.h"

GameStateDTO::GameStateDTO() {
    this->roundNumber = 1;
}

GameStateDTO::GameStateDTO(MatchSettings* settings) {
    this->settings = settings;
    this->roundNumber = 1;
    this->currentShips = 0;
}

void GameStateDTO::setSettings(MatchSettings* newSettings){
    settings = newSettings;
}
MatchSettings* GameStateDTO::getSettings() {return settings;}