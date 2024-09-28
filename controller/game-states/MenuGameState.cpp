#include "MenuGameState.h"

#include <iostream>

#include "GameState.h"
#include "OngoingGameState.h"
#include "ShutdownGameState.h"
#include "view/ViewHelper.h"

MenuGameState::MenuGameState(StateContext& context) : GameState(context) {
    this->menuView = new GameMenuView();
}

MenuGameState::~MenuGameState() {
    delete this->menuView;
}

void MenuGameState::openState() {
    ViewHelper::consoleOut("Game Menu");
    ViewHelper::consoleOut("Start\t Save\t Exit\t");
}

void MenuGameState::closeState() {
    ViewHelper::consoleOut("Quit the menu");
}

void MenuGameState::updateState() {
    menuView->drawAvailableCommands(context.currentMatch);
    std::getline(std::cin,latestCommand);
}

GameState* MenuGameState::transitToState() {
    if (latestCommand == "new"){
        return new OngoingGameState(context);
    } else if (latestCommand == "exit") {
        return new ShutdownGameState(context);
    }

    return nullptr;

    // else if (latestCommand == "exit"){
    //     newState = new ExitGamseState();
    //     return true;
    // }
    // else if (latestCommand == "save"){
    //     newState = new SaveGameState();
    //     return true;
    // }
    // return false;
}
