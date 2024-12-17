#include "ShipManager.h"
#include <map>
#include <stdexcept>

ShipManager::ShipManager(const std::map<int, int>& shipsSize)
    : shipsSizes(shipsSize)
{
    for (auto& size : shipsSizes){
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

std::vector<Ship*> ShipManager::getShips() const{
    return ships;
}

std::map<int, int> ShipManager::getShipsSizes() const {
    return shipsSizes;
}

ShipManager::ShipManager(const std::map<int, int> &shipsSizes, std::vector<Ship*> ships)
    : shipsSizes(shipsSizes)
    , ships(ships)
{}