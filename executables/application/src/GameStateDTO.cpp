#include "../include/GameStateDTO.h"

namespace cpp_warships::application {

    GameStateDTO::GameStateDTO()
        : playerManager(nullptr)
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

    std::vector<void*> GameStateDTO::initiateNewGame(MatchSettings *settings) {
        this->settings = settings;
        playerManager = new ShipManager(settings->getShipsCount());
        playerSkillManager = nullptr;
        playerField = new GameField(settings->getFieldSize(), settings->getFieldSize());
        enemyManager = new ShipManager(settings->getShipsCount());
        enemyField = new GameField(settings->getFieldSize(), settings->getFieldSize());
        shipsSizes = settings->getShipsCount();
        isFinished = false;
        fieldSize = settings->getFieldSize();
        roundNumber = 1;

        return {
            playerManager,
            playerSkillManager,
            playerField,
            enemyManager,
            enemyField
        };
    }

    GameStateDTO::GameStateDTO(const GameStateDTO &other) noexcept
        : playerManager(other.playerManager),
          playerSkillManager(other.playerSkillManager),
          playerField(other.playerField),
          enemyManager(other.enemyManager),
          enemyField(other.enemyField),
          shipsSizes(other.shipsSizes),
          settings(other.settings),
          isFinished(other.isFinished),
          fieldSize(other.fieldSize),
          roundNumber(other.roundNumber)
    {}

} // namespace cpp_warships::application