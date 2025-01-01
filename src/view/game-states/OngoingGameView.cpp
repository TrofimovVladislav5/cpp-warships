#include "OngoingGameView.h"
#include "src/library/ViewHelper.h"
#include <vector>

void OngoingGameView::displayAvailableCommands(const GameStateDTO* currentMatchData) {
    std::vector<std::string> availableCommands;
    availableCommands.emplace_back("attack - attack the opponent cell field");
    availableCommands.emplace_back("pause - pause game");
    availableCommands.emplace_back("finish - finish the game");
    ViewHelper::consoleOut("Availabale Commands \n");
    for (int i = 0;i < availableCommands.size();i++){
        ViewHelper::consoleOut(std::to_string(i + 1) + ") " + availableCommands[i]);
    }
}

void OngoingGameView::displayOpenState() {
    ViewHelper::consoleOut("Welcome to Ongoing game");
}

void OngoingGameView::displayCloseState() {
    ViewHelper::consoleOut("Quit the ongoing game");
}
