#include "GameStateDTO.h"

GameStateDTO::GameStateDTO() {
    this->roundNumber = 1;
}

GameStateDTO::~GameStateDTO() {
    delete playerManager;
    delete playerField;
    delete enemyManager;
    delete enemyField;
    delete settings;
}

GameStateDTO::GameStateDTO(MatchSettings* settings)
    : playerManager(new ShipManager(settings->getShipsCount()))
    , playerField(new GameField(settings->getFieldSize(), settings->getFieldSize()))
    , enemyManager(new ShipManager(settings->getShipsCount()))
    , enemyField(new GameField(settings->getFieldSize(), settings->getFieldSize()))
    , settings(settings)
    , shipsSizes(settings->getShipsCount())
    , fieldSize(settings->getFieldSize())
    , roundNumber(1)
{}
