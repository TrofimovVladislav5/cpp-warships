#include <string>
#include <regex>
#include "model/Ship.h"
#include "model/StateContext.h"
#include "model/Structures.h"
#include "model/GameStateDTO.h"
#include "model/MatchSettings.h"
#include "PlaceShipController.h"

#include <set>

#include "view/game-states/GameView.h"
#include "./library/parser/Parser.h"
#include "view/model/GameFieldView.h"

PlaceShipController::PlaceShipController(StateContext& context)
    : context(context)
{
    settings = context.currentMatch->getSettings();
    playerManager = settings->getPlayerManager();
    currentPlayerManager = new ShipManager({});
    gameFieldPlayer = settings->getPlayerField();
    gameFieldView = new GameFieldView(gameFieldPlayer); // TODO: Fix GameFieldView;
}

PlaceShipController::~PlaceShipController() {
    delete settings;
}

std::pair<int, int> cell(std::string coord) {
    std::smatch groups;
    std::regex_match(coord, groups, std::regex("^(\\d+),(\\d+)$"));
    return {std::stoi(groups[1].str()) , std::stoi(groups[2].str())};
}

bool isShipLengthAvailable(ShipManager& manager, ShipManager& current) {
    int countManager = 0;
    int countCurrent = 0;
    int length = current.getShips()[current.getShips().size() - 1]->getLength();
    for (auto& ship : manager.getShips()) {
        if (ship->getLength() == length) countManager++;
    }
    for (auto& ship : current.getShips()) {
        if (ship->getLength() == length) countCurrent++;
    }
    return countManager >= countCurrent;

}

bool PlaceShipController::allShipsPlaced() const {
    return playerManager->getShips().size() == currentPlayerManager->getShips().size();
}

void PlaceShipController::displayLessShips() const {
    std::map<int, int> allAvailableShips;
    std::map<int, int> usedShips;
    for (const auto& ship : playerManager->getShips()) {
        allAvailableShips[ship->getLength()]++;
    }

    for (const auto& ship : currentPlayerManager->getShips()) {
        usedShips[ship->getLength()]++;
    }
    int totalCount = 0;
    std::cout << "All available ships (length : count)" << std::endl;
    for (const auto& [length, count] : allAvailableShips) {
        totalCount = allAvailableShips[length] - usedShips[length];
        if (totalCount != 0) {
            std::cout << "Ship of length " << length << " is needed to place " << totalCount << " times" << std::endl;
        }
    }
}

void PlaceShipController::addShip(ParsedOptions options) {
    Direction direct = (options["direction"] == "vertical") ? Direction::vertical : Direction::horizontal;
    std::pair<int, int> coordinate = cell(options["cell"]);
    int length = std::stoi(options["length"]);
    currentPlayerManager->addShip(length);
    if (isShipLengthAvailable(*playerManager, *currentPlayerManager)) {
        if (bool set = gameFieldPlayer->placeShip((*currentPlayerManager)[currentPlayerManager->getShips().size() - 1], coordinate, direct); !set) {
            currentPlayerManager->removeShipNumber(currentPlayerManager->getShips().size() - 1);
        }
    }
    else {
        currentPlayerManager->removeShipNumber(currentPlayerManager->getShips().size() - 1);
    }
    std::cout << currentPlayerManager->getShips().size() << std::endl;
    gameFieldView->displayField();
    displayLessShips();
}

void PlaceShipController::removeShip(ParsedOptions options) {
    std::pair<int, int> coordinate = cell(options["cell"]);
    int index = gameFieldPlayer->removeShip(coordinate);
    if (index != - 1) {
        currentPlayerManager->removeShipNumber(index);
    }
    gameFieldView->displayField();
    //TODO gameFieldView->displayField();
}

