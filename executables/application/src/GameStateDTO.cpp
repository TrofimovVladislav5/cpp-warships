#include "../include/GameStateDTO.h"

namespace cpp_warships::application {

    GameStateDTO::GameStateDTO()
        : lastSubState("")
        , playerManager(nullptr)
        , playerSkillManager(nullptr)
        , playerField(nullptr)
        , enemyManager(nullptr)
        , enemyField(nullptr)
        , shipsSizes({})
        , settings(nullptr)
        , isFinished(false)
        , fieldSize(0)
        , roundNumber(0)
    {}

    GameStateDTO::GameStateDTO(MatchSettings* settings)
        : lastSubState("")
        , playerManager(new ShipManager(settings->getShipsCount()))
        , playerSkillManager(nullptr)
        , playerField(new GameField(settings->getFieldSize(), settings->getFieldSize()))
        , enemyManager(new ShipManager(settings->getShipsCount()))
        , enemyField(new GameField(settings->getFieldSize(), settings->getFieldSize()))
        , shipsSizes(settings->getShipsCount())
        , settings(settings)
        , isFinished(false)
        , fieldSize(settings->getFieldSize())
        , roundNumber(1)
    {}

    GameStateDTO::~GameStateDTO() {
        delete playerManager;
        delete playerField;
        delete enemyManager;
        delete enemyField;
        delete settings;
    }
} // namespace cpp_warships::application