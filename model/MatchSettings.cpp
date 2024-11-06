#include "MatchSettings.h"
#include "ShipManager.h"
#include "GameField.h"

MatchSettings::MatchSettings(int fieldSize)
    : playerField(new GameField(fieldSize, fieldSize))
    , opponentField(new GameField(fieldSize, fieldSize))
    , playerShipManager(new ShipManager({}))
    , opponentShipManager(new ShipManager({}))
    , fieldSize(fieldSize)
    , damageCount(1)
{}

MatchSettings::~MatchSettings() {
    delete playerField;
    delete opponentField;
    delete playerShipManager;
    delete opponentShipManager;
}

GameField* MatchSettings::getPlayerField() {
    return playerField;
}

GameField* MatchSettings::getOpponentField() {
    return opponentField;
}

ShipManager* MatchSettings::getPlayerManager() {
    return playerShipManager;
}

ShipManager* MatchSettings::getOpponentManager() {
    return opponentShipManager;
}

int MatchSettings::getDamage() const {
    return damageCount;
}

bool MatchSettings::getIsActiveDoubleDamage() const {
    return isActiveDoubleDamage;
}

void MatchSettings::setActiveDoubleDamage(bool isActive) {
    isActiveDoubleDamage = isActive;
}

void MatchSettings::setPlayerManager(const std::vector<int>& shipsLengths) {
    delete playerShipManager;
    playerShipManager = new ShipManager(shipsLengths);

    delete opponentShipManager;
    opponentShipManager = new ShipManager(shipsLengths);
}

void MatchSettings::setFieldSize(int size) {
    this->fieldSize = size;

    if (playerField != nullptr) {
        delete playerField;
    }
    if (opponentField != nullptr) {
        delete opponentField;
    }

    playerField = new GameField(fieldSize, fieldSize);
    opponentField = new GameField(fieldSize, fieldSize);
}

void MatchSettings::setDamageCount(int damage) {
    damageCount = damage;
}

void MatchSettings::setScanCoordinates(const std::pair<int, int> &scanCoordinate) {
    this->scanCoordinate = scanCoordinate;
}

const std::pair<int, int> &MatchSettings::coordinateToScan() {
    return scanCoordinate;
}