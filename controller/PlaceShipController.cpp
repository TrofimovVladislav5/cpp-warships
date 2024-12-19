#include "PlaceShipController.h"

#include <string>
#include <regex>
#include "../model/Structures.h"

#include <random>

#include "SubStateContext.h"
#include "ViewHelper.h"
#include "../library/TypesHelper.h"
#include "../library/parser/Parser.h"
#include "../view/GameFieldView.h"
#include "exceptions/ShipPlacementException.h"


PlaceShipController::PlaceShipController(GameStateDTO* dto, ShipManager* manager)
    : currentField(new GameField(dto->fieldSize, dto->fieldSize))
    , manager(manager)
    , currentManager(new ShipManager({}))
{
    eraseShipsDictionary();
}

PlaceShipController::~PlaceShipController() {
    delete manager;
    delete currentManager;
    delete currentField;
}

void PlaceShipController::eraseShipsDictionary() {
    availableLengthShips.clear();

    for (const auto& ship : manager->getShips()) {
        int length = ship->getLength();
        availableLengthShips[length]++;
    }
}

std::map<int, int> PlaceShipController::getAvailableLengthShips() {
    return availableLengthShips;
}

bool PlaceShipController::isShipLengthAvailable(int length) {
    auto it = availableLengthShips.find(length);
    if (it == availableLengthShips.end()) {
        return false;
    }
    return it->second > 0;
}

bool PlaceShipController::allShipsPlaced() {
    return currentManager->getShips().size() == manager->getShips().size();
}

void PlaceShipController::addShip(ParsedOptions options) {
    Direction direct = (options["direction"] == "vertical") ? Direction::vertical : Direction::horizontal;

    std::pair<int, int> coordinate = TypesHelper::cell(options["cell"]);
    int length = std::stoi(options["length"]);
    if (!isShipLengthAvailable(length)) {
        throw ShipPlacementException("(where add ship) no more ships of length " + options["length"] + " available.");
    }
    if (!currentField->canPlaceShip(coordinate, direct, length)) {
        throw ShipPlacementException("(where add ship) cannot place ship at the specified cell " + options["cell"]
                            + ".\nEither the ship intersects with another or cell out of field bounds.");
    }
    currentManager->addShip(length);
    currentField->placeShip(currentManager->getShips().back(), coordinate, direct);
    availableLengthShips[length]--;
}

void PlaceShipController::handleRemoveShip(std::pair<int, int> coordinate) {
    std::pair<int, int> result = currentField->removeShip(coordinate);
    if (result.first == - 1) {
        throw ShipPlacementException(
            std::string("No ship found at field cell ")
            .append(std::to_string(coordinate.first))
            .append(std::to_string(coordinate.second))
        );
    }
    int shipIndex = result.second;
    int shipLength = currentManager->getShips()[shipIndex]->getLength();
    availableLengthShips[shipLength]++;
    currentManager->removeShipNumber(shipIndex);
}

void PlaceShipController::removeShip(ParsedOptions options) {
    std::pair<int, int> coordinate = TypesHelper::cell(options["cell"]);
    handleRemoveShip(coordinate);
}

GameField * PlaceShipController::getCurrentField() const {
    return currentField;
}

ShipManager * PlaceShipController::getCurrentManager() const {
    return currentManager;
}

void PlaceShipController::placeShipsRandomly() {
    std::random_device random_device;
    std::mt19937 mersenneEngine{random_device()};
    std::uniform_int_distribution distY{0, currentField->getHeight() - 1};
    std::uniform_int_distribution distX{0, currentField->getWidth() - 1};
    std::uniform_int_distribution distDirection{0, 1};

    eraseShipsDictionary();
    currentManager->clear();
    currentField->clear();

    std::vector<Ship*> ships = manager->getShips();
    std::sort(ships.begin(), ships.end(), [](Ship* a, Ship* b) {
        return a->getLength() > b->getLength();
    });

    for (auto ship : ships) {
        bool placed = false;
        constexpr int MAX_ITERATIONS = 100;
        int iterations = 0;

        while (!placed && iterations <= MAX_ITERATIONS) {
            const std::pair<int, int> coordinate = std::make_pair(distX(mersenneEngine), distY(mersenneEngine));
            Direction direct = (distDirection(mersenneEngine) == 0) ? Direction::horizontal : Direction::vertical;
            if (currentField->canPlaceShip(coordinate, direct, ship->getLength())) {
                currentManager->addShip(ship->getLength());
                currentField->placeShip(ship, coordinate, direct);
                placed = true;
            }
            iterations++;
        }

        if (iterations > MAX_ITERATIONS) {
            ViewHelper::errorOut("Were not able to resolve ship placement. Please, try one more time");
            currentManager->clear();
            currentField->clear();
            return;
        }

        availableLengthShips[ship->getLength()]--;
    }

    ships.clear();
}