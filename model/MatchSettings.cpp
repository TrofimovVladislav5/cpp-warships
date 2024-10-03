#include "MatchSettings.h"
#include "ShipManager.h"
#include "GameField.h"

MatchSettings::MatchSettings(int fieldSize){
    this->fieldSize = fieldSize;
    this->playerField = new GameField(fieldSize, fieldSize);
    this->opponentField = new GameField(fieldSize, fieldSize);
    this->playerShipManager = new ShipManager();
    this->opponentShipManager = new ShipManager();
}

MatchSettings::~MatchSettings(){
    delete playerField;
    delete opponentField;
    delete playerShipManager;
    delete opponentShipManager;
}

GameField* MatchSettings::getPlayerField() { return playerField;}
GameField* MatchSettings::getOpponentField() {return opponentField;}
ShipManager* MatchSettings::getPlayerManager() {return playerShipManager;}
ShipManager* MatchSettings::getOpponentShipManager() {return opponentShipManager;}
void MatchSettings::setFieldSize(int size) { 
    this->fieldSize = size;
    delete playerField;
    delete opponentField;
    playerField = new GameField(fieldSize, fieldSize);
    opponentField = new GameField(fieldSize, fieldSize);
}
int MatchSettings::getFieldSize() {return fieldSize;}