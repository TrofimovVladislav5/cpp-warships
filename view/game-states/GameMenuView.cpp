#include "GameMenuView.h"
#include "library/ViewHelper.h"
#include <vector>

void GameMenuView::displayAvailableCommands(const GameStateDTO* currentMatch) {
    std::vector<std::string> availableCommands;

    if (currentMatch) {
        availableCommands = {
            {"continue - return to game"},
            {"save - WIP"},
            {"load - WIP"},
            {"new - start new game"},
            {"exit - leave current match"}
        };
    } else {
        availableCommands = {
            {"load - WIP"},
            {"new - start new game"},
            {"exit - leave the game"},
        };
    }

    ViewHelper::consoleOut("Available commands: \n");
    for (int i = 0; i < availableCommands.size(); ++i) {
        ViewHelper::consoleOut(std::to_string(i + 1) + ") " + availableCommands[i] );
    }
}

void GameMenuView::displayOpenState() {
    ViewHelper::consoleOut("Game Menu");
    ViewHelper::consoleOut("Start\tSave\tExit\n");
}

void GameMenuView::displayCloseState() {
    ViewHelper::consoleOut("Quit the menu");
}