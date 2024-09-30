#include <string>
#include "ShipManager.h"
#include "Ship.h"
#include "Structures.h"
#include "view/ViewHelper.h"

void ShipManager::addShip(Ship* ship){
    ships.emplace_back(*ship);
}

void ShipManager::printStateShips() {
    ViewHelper::consoleOut("Ships segments info");

    for (int i = 0; i < ships.size(); i++) {
        std::string segmenInfo;  
        for (SegmentState segment : ships[i].getSegments()){
            switch (segment) {
                case SegmentState::INT:
                    segmenInfo += "INT ";
                    break;
                case SegmentState::DAMAGED:
                    segmenInfo += "DAMAGED ";
                    break;
                case SegmentState::DESTROYED:
                    segmenInfo += "DESTROYED ";
                    break;
            }
            ViewHelper::consoleOut("Ship " + std::to_string(i + 1) + 
                               " length " + std::to_string(ships[i].getLength()) + 
                               ": " + segmenInfo); 
        }
    }
}