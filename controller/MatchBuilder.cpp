#include "MatchBuilder.h"

#include "ViewHelper.h"
#include "game-states/OngoingGameState.h"
#include "ongoing-game-substates/BattleOngoingGameSubState.h"
#include "ongoing-game-substates/InitiateOngoingGameSubState.h"
#include "ongoing-game-substates/NewMatchSettingsSubState.h"
#include "save/GameSaveCreator.h"

OngoingGameSubState* MatchBuilder::initializeNewMatch() {
    SubStateContext* context = new SubStateContext(currentData);

    if (isLoadedFromTemplate) {
        return new InitiateOngoingGameSubState(context);
    } else {
        return new NewMatchSettingsSubState(context);
    }
}

OngoingGameSubState* MatchBuilder::loadSavedMatch() {
    SubStateContext* context = new SubStateContext(currentData);

    try {
        int placedShipsAmount = static_cast<int>(currentData->playerField->getShipsCoordinateMap().size());
        int expectedShipsAmount = static_cast<int>(currentData->playerManager->getShips().size());

        if (placedShipsAmount == expectedShipsAmount) {
            return new BattleOngoingGameSubState(context);
        } else {
            return new InitiateOngoingGameSubState(context);
        }
    } catch (std::exception &e) {
        ViewHelper::errorOut("Something went wrong while trying to create a new match from the loaded save", e);
        return nullptr;
    }
}

MatchBuilder::MatchBuilder()
    : isLoaded(false)
    , isLoadedFromTemplate(false)
    , currentData(nullptr)
{}

void MatchBuilder::newGame(bool fromTemplate) {
    try {
        MatchSettings matchSettings({
            {1, 4},
            {2, 3},
            {3, 2},
            {4, 1}
        }, 10);

        currentData = new GameStateDTO(&matchSettings);
        isLoaded = false;
        isLoadedFromTemplate = fromTemplate;
    } catch (std::exception &e) {
        ViewHelper::errorOut("Something went wrong while creating new match settings", e);
        currentData = nullptr;
    }
}

void MatchBuilder::loadSave(const std::string &filename) {
    try {
        GameSaveCreator saveCreator;
        currentData = saveCreator.loadSave(filename);
        isLoaded = true;
    } catch (std::exception &e) {
        ViewHelper::errorOut("Something went wrong while loading the save file at path: " + filename, e);
        currentData = nullptr;
    }
}

std::function<OngoingGameSubState*()> MatchBuilder::getStateBuilder() {
    return isLoaded
        ? std::bind(&MatchBuilder::loadSavedMatch, this)
        : std::bind(&MatchBuilder::initializeNewMatch, this);
}
