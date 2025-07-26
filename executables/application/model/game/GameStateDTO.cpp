#include "GameStateDTO.h"

namespace cpp_warships::application {

    GameStateDTO::GameStateDTO()
        : playerManager(nullptr)
        , playerField(nullptr)
        , enemyManager(nullptr)
        , enemyField(nullptr)
        , settings(nullptr)
        , shipsSizes({})
        , fieldSize(0)
        , roundNumber(0)
        , lastSubState("")
        , playerSkillManager(nullptr)
        , isFinished(false)
    {}

    GameStateDTO::GameStateDTO(MatchSettings* settings)
        : playerManager(new ShipManager(settings->getShipsCount()))
        , playerField(new GameField(settings->getFieldSize(), settings->getFieldSize()))
        , enemyManager(new ShipManager(settings->getShipsCount()))
        , enemyField(new GameField(settings->getFieldSize(), settings->getFieldSize()))
        , settings(settings)
        , shipsSizes(settings->getShipsCount())
        , fieldSize(settings->getFieldSize())
        , roundNumber(1)
        , lastSubState("")
        , playerSkillManager(nullptr)
        , isFinished(false)
    {}

    GameStateDTO::~GameStateDTO() {
        delete playerManager;
        delete playerField;
        delete enemyManager;
        delete enemyField;
        delete settings;
    }
} // namespace cpp_warships::application