#include "PlaceShipController.h"

#include <string>
#include <regex>
#include "../model/Ship.h"
#include "../model/Structures.h"
#include "../model/MatchSettings.h"

#include <random>

#include "../library/defaults/ShipPlacementException.h"
#include "../library/TypesHelper.h"
#include "../library/parser/Parser.h"
#include "../view/model/GameFieldView.h"

PlaceShipController::PlaceShipController(MatchSettings& settings)
    : settings(settings)
    , currentPlayerManager(new ShipManager({}))
    , currentComputerManager(new ShipManager({}))
    , gameFieldView(new GameFieldView(settings.getPlayerField()))
{
    for (const auto& ship : settings.getPlayerManager()->getShips()) {
        int length = ship->getLength();
        availableLengthShips[length]++;
    }
}

PlaceShipController::~PlaceShipController() {
    for (int i = 0; i < currentPlayerManager->getShips().size();i++) {
        delete (*currentPlayerManager)[i];
        delete (*currentComputerManager)[i];
    }
}

bool PlaceShipController::isShipLengthAvailable(int length) {
    auto it = availableLengthShips.find(length);
    if (it == availableLengthShips.end()) {
        return false;
    }
    return it->second > 0;
}

bool PlaceShipController::allShipsPlaced() {
    return !settings.getPlayerManager()->getShips().empty();
    // auto original = settings.getPlayerManager()->getShips();
    // auto current = currentPlayerManager->getShips();
    // return original.size() == current.size();
}

void PlaceShipController::displayLessShips() const {
    std::cout << "All available ships (length : count)" << std::endl;
    for (const auto& pair : availableLengthShips) {
        if (pair.second != 0)
            std::cout << "Ship of length " << pair.first << " is needed to place " << pair.second << " times" << std::endl;
    }
}

void PlaceShipController::addShip(ParsedOptions options) {
    Direction direct = (options["direction"] == "vertical") ? Direction::vertical : Direction::horizontal;
    std::pair<int, int> coordinate = TypesHelper::cell(options["cell"]);
    int length = std::stoi(options["length"]);
    if (!isShipLengthAvailable(length)) {
        throw ShipPlacementException("(where add ship) no more ships of length " + options["length"] + " available.");
    }
    std::cout << currentPlayerManager->getShips().size() << std::endl;
    if (!settings.getPlayerField()->canPlaceShip(coordinate, direct, length)) {
        throw ShipPlacementException("(where add ship) cannot place ship at the specified cell " + options["cell"]
                            + ".\nEither the ship intersects with another or cell out of field bounds.");
    }
    currentPlayerManager->addShip(length);
    settings.getPlayerField()->placeShip(currentPlayerManager->getShips().back(), coordinate, direct);
    availableLengthShips[length]--;
    gameFieldView->displayField(false);
    displayLessShips();
}

void PlaceShipController::removeShip(ParsedOptions options) {
    std::pair<int, int> coordinate = TypesHelper::cell(options["cell"]);
    int index = settings.getPlayerField()->removeShip(coordinate);
    if (index == - 1) {
        throw ShipPlacementException("(where remove ship) no ship was found at field cells " + options["cell"]);
    }
    availableLengthShips[currentPlayerManager->getShips()[index]->getLength()]++;
    currentPlayerManager->removeShipNumber(index);
    gameFieldView->displayField(false);
    displayLessShips();
}

void PlaceShipController::placeShipComputer() {
    std::random_device random_device;
    std::mt19937 mersenneEngine{random_device()};
    std::uniform_int_distribution<int> distY{0, settings.getPlayerField()->getHeight() - 1};
    std::uniform_int_distribution<int> distX{0, settings.getPlayerField()->getWidth() - 1};
    std::uniform_int_distribution<int> distDirection{0, 1};
    for (auto ship : settings.getOpponentManager()->getShips()) {
        while (true) {
            const std::pair<int, int> coordinate = std::make_pair(distX(mersenneEngine), distY(mersenneEngine));
            Direction direct = (distDirection(mersenneEngine) == 0) ? Direction::horizontal : Direction::vertical;
            if (settings.getOpponentField()->canPlaceShip(coordinate, direct, ship->getLength())) {
                currentComputerManager->addShip(ship->getLength());
                settings.getOpponentField()->placeShip(ship, coordinate, direct);
                break;
            }
        }
    }
}

