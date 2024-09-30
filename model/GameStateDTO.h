#pragma once
#include "GameField.h"
#include "ShipManager.h"
#include "StateContext.h"
#include "Ship.h"

class GameStateDTO {
public:
    GameStateDTO();
    GameStateDTO(GameSettings settings);
    int roundNumber;
};