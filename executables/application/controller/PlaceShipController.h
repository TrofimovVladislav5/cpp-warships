#pragma once

#include <cpp_warships/input_parser/include/model/Parser.h>
#include "../model/StateContext.h"

class PlaceShipController {
private:
    GameField* currentField;
    ShipManager* manager;
    ShipManager* currentManager;
    std::map<int, int> availableLengthShips;

    void handleRemoveShip(std::pair<int, int> coordinate);
    bool isShipLengthAvailable(int length);
    void eraseShipsDictionary();

public:
    bool allShipsPlaced();
    explicit PlaceShipController(GameStateDTO* context, ShipManager* manager);
    ~PlaceShipController();
    std::map<int, int> getAvailableLengthShips();
    void addShip(cpp_warships::input_parser::ParsedOptions options);
    void removeShip(cpp_warships::input_parser::ParsedOptions options);
    void placeShipsRandomly();
    [[nodiscard]] GameField* getCurrentField() const;
    [[nodiscard]] ShipManager* getCurrentManager() const;
};