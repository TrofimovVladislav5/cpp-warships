#include "GameStateDTO.h"

GameStateDTO::GameStateDTO() {
    this->roundNumber = 1;
}

GameStateDTO::GameStateDTO(MatchSettings* settings)
    : playerManager(new ShipManager(settings->getShipsCount()))
    , playerField(new GameField(settings->getFieldSize(), settings->getFieldSize()))
    , enemyManager(new ShipManager(settings->getShipsCount()))
    , enemyField(new GameField(settings->getFieldSize(), settings->getFieldSize()))
    , settings(settings)
    , shipsSize(settings->getShipsCount())
    , fieldSize(settings->getFieldSize())
    , roundNumber(1)
    , currentShips(0)
    , playerSkillManager(nullptr)
{}
