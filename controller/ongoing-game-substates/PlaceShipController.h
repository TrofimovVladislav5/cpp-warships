#pragma once
#include "model/StateContext.h"
#include "library/parser/Parser.h"
#include "view/model/GameFieldView.h"
class PlaceShipController {
private:
    MatchSettings* settings;
    ShipManager* playerManager;
    ShipManager* currentPlayerManager;
    ShipManager* computerManager;
    ShipManager* currentComputerManager;
    GameField* gameFieldPlayer;
    GameField* gameFieldComputer;
    GameFieldView* gameFieldView;
    std::map<int, int> availableLengthShips;
    StateContext context;
    bool isShipLengthAvailable(int length);
public:
    bool allShipsPlaced();
    PlaceShipController(StateContext& context);
    ~PlaceShipController();
    void addShip(ParsedOptions options);
    void removeShip(ParsedOptions options);
    void displayLessShips() const;
    void placeShipComputer();
};