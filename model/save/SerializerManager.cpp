#include "SerializerManager.h"

#include "SerializerFactory.h"

json SerializerManager::serialize(const GameStateDTO &object) {
    json shipsSizesJson = shipsSizesToJson(object.shipsSizes);
    json playerManagerJson = vectorShipsToJson(object.playerManager->getShips());
    json enemyManagerJson = vectorShipsToJson(object.enemyManager->getShips());
    json j = {
        {"playerShips", playerManagerJson},
        {"enemyShips", enemyManagerJson},
        {"shipsSizes", shipsSizesJson}
    };
    return j;
}

void SerializerManager::deserialize(const json &j, GameStateDTO &object) {
    std::map<int, int> shipsSizes = shipSizesFromJson(j.at("shipsSizes"));
    std::vector<Ship*> playerShips = vectorShipsFromJson(j.at("playerShips"));
    std::vector<Ship*> enemyShips = vectorShipsFromJson(j.at("enemyShips"));
    ShipManager* playerManager = new ShipManager(shipsSizes, playerShips);
    ShipManager* enemyManager = new ShipManager(shipsSizes, enemyShips);
    object.playerManager = playerManager;
    object.enemyManager = enemyManager;
}
