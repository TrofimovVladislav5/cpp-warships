#include "InitiateOngoingGameView.h"

#include "library/ViewHelper.h"

void InitiateOngoingGameView::displayAvailableCommands(const GameStateDTO* currentMatchData) {
    const std::vector<std::string> availableCommands = {
            {"Add\n"
             "   Usage: add --length <length> --direction <horizontal|vertical> --cell <x,y>"},
            {"Remove\n"
             "   Usage: remove --cell <x,y>"},
            {"Confirm\n"
             "   Usage: confirm"},
    };
    ViewHelper::consoleOut("Available commands\n");
    for (int i = 0; i < availableCommands.size(); i++) {
        ViewHelper::consoleOut(std::to_string(i + 1) + ") " + availableCommands[i]);
    }
}

void InitiateOngoingGameView::displayOpenState() {
    ViewHelper::consoleOut("Welcome to placement ships phase\n");
    const std::vector<std::string> placementRules = {
            {"Ships must be placed within the boundaries of the field"},
            {"Ships cannot overlap or touch each other, even at corners."},
            {"Ships can be added in either horizontal or vertical orientation."},
            {"Each ship has a defined length, from 1 to 4 cells."}};
    ViewHelper::consoleOut("Placement Rules:\n");
    for (int i = 0; i < placementRules.size(); i++) {
        ViewHelper::consoleOut(std::to_string(i + 1) + ") " + placementRules[i]);
    }
}

void InitiateOngoingGameView::displayCloseState() {
    ViewHelper::consoleOut("The Phase of placement ships is completed");
}
