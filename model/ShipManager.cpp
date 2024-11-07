#include "ShipManager.h"
#include <map>


ShipManager::ShipManager(const std::map<int, int>& shipsSize)
    : shipsSize(shipsSize)
{
    for (auto& size : shipsSize){
        for (int i = 0; i < size.second; i++) {
            Ship* currentShip = new Ship(size.first);
            ships.push_back(currentShip);
        }
    }
}

ShipManager::~ShipManager(){
    for (auto& ship : ships){
        delete ship;
    }
}

Ship* ShipManager::operator[](int index){
    if (index < 0 || index >= ships.size()) {
        throw std::out_of_range("Invalid index error");
    }
    return ships[index];
}

void ShipManager::addShip(int size){
    Ship* newShip = new Ship(size);
    ships.push_back(newShip);
}

void ShipManager::removeShipNumber(int indexRemoving){
    if (indexRemoving < 0 || indexRemoving >= ships.size()){
        throw std::out_of_range("Invalid Index for removing ship");
    }
    ships.erase(ships.begin() + indexRemoving);
}

void ShipManager::clear() {
    ships.clear();
}

std::vector<Ship*> ShipManager::getShips() {
    return ships;
}
