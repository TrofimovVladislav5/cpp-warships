#include <string>
#include "GameState.h"
#include "model/Structures.h"
#include "model/GameStateDTO.h"
#include "view/ViewHelper.h"
#include "PlaceShipController.h"

PlaceShipController::PlaceShipController() : GameState(context) {
    this->minimalCountShips = 10;
    this->minimalFieldSize = 10;
};

PlaceShipController::placementShip(){
    ViewHelper::consoleOut("Enter a value for the field size, 
        if you do not enter it, the standard field will be 10 x 10");
    std::string potentialSize;
    if (!isdigit(std::stoi(potentialSize))){
        //TODO: Class Exceptions
    }
    int size = std::stoi(potentialSize);
    ViewHelper::consoleOut("Enter a direction placeShip: horizontal, vertical");
    Direction direction;
    std::string potentialDirection;
    std::getline(std::cin, potentialDirection);
    if (potentialDirection == "horizontal"){
        direction = Direction::HORIZONTAL;
    }
    else if (potentialDirection == "vertical"){
        direction = Direction::VERTICAL;
    }
    ViewHelper::consoleOut("Enter a length of ship 1-4");
    int length;
    std::cin >> length;
}