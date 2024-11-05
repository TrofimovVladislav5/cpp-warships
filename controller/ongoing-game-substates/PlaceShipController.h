#pragma once
#include "model/StateContext.h"
#include "library/parser/Parser.h"
#include "view/model/GameFieldView.h"
class PlaceShipController {
private:
    MatchSettings* settings;
    ShipManager* currentPlayerManager;
    ShipManager* currentComputerManager;
    GameFieldView* gameFieldView;
    std::map<int, int> availableLengthShips;
    StateContext context;
    bool isShipLengthAvailable(int length);
public:
    bool allShipsPlaced();
    explicit PlaceShipController(StateContext& context);
    ~PlaceShipController();
    void addShip(ParsedOptions options);
    void removeShip(ParsedOptions options);
    void displayLessShips() const;
    void placeShipComputer();
};