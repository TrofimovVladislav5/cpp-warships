#include <string>
#include <regex>
#include "model/Ship.h"
#include "model/StateContext.h"
#include "model/Structures.h"
#include "model/GameStateDTO.h"
#include "model/MatchSettings.h"
#include "PlaceShipController.h"
#include "view/GameView.h"

PlaceShipController::PlaceShipController(StateContext& context)
    : context(context)
    , minimalCountShips(10)
{}

PlaceShipController::~PlaceShipController(){

}

void PlaceShipController::addShip(){
    // MatchSettings* settings;
    // Parser* parser = new Parser();
    // if (context.currentMatch == nullptr){
    //     settings = new MatchSettings();
    //     context.currentMatch = new GameStateDTO(settings);
    // }
    // else{
    //     settings = context.currentMatch->getSettings();
    // }
    // int length = parser->parseLengthShip();
    // Direction direction = parser->parseDirectionShip();
    // std::pair<int, int> coordinate = parser->parseCoordinateCell();
    // Ship* ship = new Ship(length, direction);
    // ShipManager* playerManager = settings->getPlayerManager();
    // GameField* gameField = settings->getPlayerField();
    // if (gameField->placeShip(ship, coordinate.first, coordinate.second)){
    //     playerManager->addShip(ship);
    //     playerManager->printStateShips();
    // }
    // gameField->printGameField();
    // delete parser;
}

void PlaceShipController::deleteShip(){
    // MatchSettings* settings;
    // Parser* parser = new Parser();
    // if (context.currentMatch == nullptr){
    //     settings = new MatchSettings();
    //     context.currentMatch = new GameStateDTO(settings);
    // }
    // else{
    //     settings = context.currentMatch->getSettings();
    // }
    // std::pair<int, int> coordinate = parser->parseCoordinateCell();
    // GameField* gameField = settings->getPlayerField();
    // gameField->removeShip(coordinate.first, coordinate.second);
    // gameField->printGameField();
}