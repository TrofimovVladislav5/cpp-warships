#include "CommonGameSettingsView.h"
#include "library/ViewHelper.h"

void CommonGameSettingsView::displayAvailableCommands(const GameStateDTO* currentMatch) {
    // ViewHelper::consoleOut("rules availablecommands");
    std::vector<std::string> availableCommands = {
        {"set - field size must be set between 10 and 25"},
        {"confirm - confirm to set the game field size"}
    };
    ViewHelper::consoleOut("Available Commands \n");
    for (int i = 0; i < availableCommands.size(); ++i) {
        ViewHelper::consoleOut(std::to_string(i + 1) + ") " + availableCommands[i] );
    }
}


void CommonGameSettingsView::displayOpenState() {
    ViewHelper::consoleOut("Welcome to the Common Game Settings! \n"
                           "The number of ships and their lengths will be optimally calculated based on the field size you set.\n");
}

void CommonGameSettingsView::displayCloseState() {
    ViewHelper::consoleOut("Game settings is set");
}