#include "MatchBuilder.h"

#include "ViewHelper.h"
#include "game-states/OngoingGameState.h"
#include "ongoing-game-substates/BattleOngoingGameSubState.h"
#include "ongoing-game-substates/InitiateOngoingGameSubState.h"
#include "ongoing-game-substates/NewMatchSettingsSubState.h"
#include "save/GameSaveCreator.h"

OngoingGameSubState* MatchBuilder::initializeNewMatch() {
    SubStateContext* context = new SubStateContext(currentData, reader);

    if (isLoadedFromTemplate) {
        return new InitiateOngoingGameSubState(context);
    } else {
        return new NewMatchSettingsSubState(context);
    }
}

OngoingGameSubState* MatchBuilder::loadSavedMatch() {
    SubStateContext* context = new SubStateContext(currentData, reader);

    try {
        if (currentData->playerField && currentData->playerManager &&
            currentData->playerField->getShipsCoordinateMap().size() ==
                    currentData->playerManager->getShips().size())
            return new BattleOngoingGameSubState(context);

        return new InitiateOngoingGameSubState(context);
    } catch (std::exception& e) {
        ViewHelper::errorOut(
                "Something went wrong while trying to create a new match from the loaded save", e);
        return nullptr;
    }
}

MatchBuilder::MatchBuilder(InputReader<>* reader)
    : isLoaded(false), isLoadedFromTemplate(false), currentData(nullptr), reader(reader) {}

void MatchBuilder::newGame(bool fromTemplate) {
    try {
        MatchSettings matchSettings(defaultSettings.shipsCount, defaultSettings.fieldSize);

        currentData = new GameStateDTO(&matchSettings);
        isLoaded = false;
        isLoadedFromTemplate = fromTemplate;
    } catch (std::exception& e) {
        ViewHelper::errorOut("Something went wrong while creating new match settings", e);
        currentData = nullptr;
    }
}

bool MatchBuilder::loadSave(const std::string& filename) {
    try {
        GameSaveCreator saveCreator;
        currentData = saveCreator.loadSave(filename);
        isLoaded = currentData != nullptr;
    } catch (std::exception& e) {
        ViewHelper::errorOut(
                "Something went wrong while loading the save file at path: " + filename, e);
        currentData = nullptr;
        isLoaded = false;
    }

    return isLoaded;
}

void MatchBuilder::printBattleScreenshot() {
    if (isLoaded && currentData) {
        BattleView view = BattleView(currentData);
        view.printBattleState();
    } else if (currentData && isLoadedFromTemplate) {
        MatchSettings matchSettings(defaultSettings.shipsCount, defaultSettings.fieldSize);

        ViewHelper::consoleOut("Current save is loaded from template with the following settings:");
        ViewHelper::consoleOut("Field size: ", 1);
        ViewHelper::consoleOut(std::to_string(matchSettings.getFieldSize()), 2);
        ViewHelper::consoleOut("Ships sizes: ", 1);
        for (auto& ship : matchSettings.getShipsCount()) {
            ViewHelper::consoleOut("Ship of size " + std::to_string(ship.first) + " - " +
                                           std::to_string(ship.second),
                                   2);
        }
        ViewHelper::consoleOut("Damage count: ", 1);
        ViewHelper::consoleOut(std::to_string(matchSettings.damageCount), 2);
    } else if (currentData) {
        ViewHelper::consoleOut(
                "Empty save is currently handled. If you start a game, you will need to initialize "
                "all the settings.");
    } else {
        ViewHelper::consoleOut("No data is currently handled.");
    }
}

std::function<OngoingGameSubState*()> MatchBuilder::getStateBuilder() {
    return isLoaded ? std::bind(&MatchBuilder::loadSavedMatch, this)
                    : std::bind(&MatchBuilder::initializeNewMatch, this);
}
