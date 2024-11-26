#pragma once
#include "../model/StateContext.h"
#include "../library/parser/Parser.h"


class PlaceShipController {
private:
    GameField* currentField;
    ShipManager* manager;
    ShipManager* currentManager;
    std::map<int, int> availableLengthShips;
    bool isShipLengthAvailable(int length);
public:
    bool allShipsPlaced();
    explicit PlaceShipController(GameStateDTO* context, ShipManager* manager);
    ~PlaceShipController();
    std::map<int, int> getAvailableLengthShips();
    bool compareByLengthDesc(Ship* a, Ship* b);
    void addShip(ParsedOptions options);
    void removeShip(ParsedOptions options);
    void placeShipsRandomly();
    [[nodiscard]] GameField* getCurrentField() const;
    [[nodiscard]] ShipManager* getCurrentManager() const;
};