#include "GameMenuView.h"

#include <vector>

#include "ViewHelper.h"

void GameMenuView::drawAvailableCommands(const GameStateDTO* currentMatch) {
    std::vector<std::string> availableCommands;

    if (currentMatch) {
        availableCommands.emplace_back("continue - return to game");
        availableCommands.emplace_back("save - WIP");
        availableCommands.emplace_back("load - WIP");
        availableCommands.emplace_back("new - start new game");
        availableCommands.emplace_back("exit - leave current match");
    } else {
        availableCommands.emplace_back("load - WIP");
        availableCommands.emplace_back("new - start new game");
        availableCommands.emplace_back("exit - leave the game");
    }

    ViewHelper::consoleOut("Available commands: \n");
    for (int i = 0; i < availableCommands.size(); ++i) {
        ViewHelper::consoleOut(std::to_string(i + 1) + ')' + ' ' + availableCommands[i] );
    }
}
