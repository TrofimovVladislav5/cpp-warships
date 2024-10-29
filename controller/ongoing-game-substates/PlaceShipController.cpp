#include <string>
#include <regex>
#include "model/Ship.h"
#include "model/StateContext.h"
#include "model/Structures.h"
#include "model/GameStateDTO.h"
#include "model/MatchSettings.h"
#include "PlaceShipController.h"

#include "ShipPlacementException.h"
#include "./library/parser/Parser.h"
#include "view/model/GameFieldView.h"

PlaceShipController::PlaceShipController(StateContext& context)
    : context(context)
{
    settings = context.currentMatch->getSettings();
    playerManager = settings->getPlayerManager();
    computerManager = settings->getOpponentManager();
    currentPlayerManager = new ShipManager({});
    currentComputerManager = new ShipManager({});
    gameFieldPlayer = settings->getPlayerField();
    gameFieldComputer = settings->getOpponentField();
    gameFieldView = new GameFieldView(gameFieldPlayer); // TODO: Fix GameFieldView;

    for (const auto& ship : playerManager->getShips()) {
        int length = ship->getLength();
        availableLengthShips[length]++;
    }
}

PlaceShipController::~PlaceShipController() {
    for (int i = 0; i < currentPlayerManager->getShips().size();i++) {
        delete (*currentPlayerManager)[i];
    }
    delete settings;
}

std::pair<int, int> cell(const std::string& coord) {
    std::smatch groups;
    std::regex_match(coord, groups, std::regex("^(\\d+),(\\d+)$"));
    return {std::stoi(groups[1].str()) , std::stoi(groups[2].str())};
}

bool PlaceShipController::isShipLengthAvailable(int length) {
    auto it = availableLengthShips.find(length);
    if (it == availableLengthShips.end()) {
        return false;
    }
    return it->second > 0;
}

bool PlaceShipController::allShipsPlaced() {
    this->placeShipComputer();
    return playerManager->getShips().size() == currentPlayerManager->getShips().size()
        && computerManager->getShips().size() == currentComputerManager->getShips().size();
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
    std::pair<int, int> coordinate = cell(options["cell"]);
    int length = std::stoi(options["length"]);
    if (!isShipLengthAvailable(length)) {
        throw ShipPlacementException("(where add ship) no more ships of length " + options["length"] + " available.");
    }
    if (!gameFieldPlayer->canPlaceShip(coordinate, direct, length)) {
        throw ShipPlacementException("(where add ship) cannot place ship at the specified cell " + options["cell"]
                            + ".\nEither the ship intersects with another or cell out of field bounds.");
    }
    currentPlayerManager->addShip(length);
    gameFieldPlayer->placeShip(currentPlayerManager->getShips().back(), coordinate, direct);
    availableLengthShips[length]--;
    gameFieldView->displayField(false);
    displayLessShips();
}

void PlaceShipController::removeShip(ParsedOptions options) {
    std::pair<int, int> coordinate = cell(options["cell"]);
    int index = gameFieldPlayer->removeShip(coordinate);
    if (index == - 1) {
        throw ShipPlacementException("(where remove ship) no ship was found at field cells " + options["cell"]);
    }
    availableLengthShips[currentPlayerManager->getShips()[index]->getLength()]++;
    currentPlayerManager->removeShipNumber(index);
    gameFieldView->displayField(false);
    displayLessShips();
}


void PlaceShipController::placeShipComputer() {
    std::srand(std::time(0));
    int flag = 0;
    if (computerManager->getShips().size() > currentComputerManager->getShips().size()) {
        for (auto ship : computerManager->getShips()) {
            while (flag != 1) {
                std::pair<int, int> coordinate = std::make_pair(std::rand() % gameFieldComputer->getWidth(), std::rand() % gameFieldComputer->getHeight());
                Direction direct = (std::rand() % 2 == 0) ? Direction::horizontal : Direction::vertical;
                if (gameFieldComputer->canPlaceShip(coordinate, direct, ship->getLength())) {
                    currentComputerManager->addShip(ship->getLength());
                    gameFieldComputer->placeShip(ship, coordinate, direct);
                    break;
                }
            }
        }
    }
    GameFieldView oppon(gameFieldComputer);
    oppon.displayField(false);
}

