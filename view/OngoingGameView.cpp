#include "OngoingGameView.h"
#include "../library/ViewHelper.h"
#include <vector>


void OngoingGameView::printGameCommands() {
    //TODO: implement list of commands
    std::vector<std::string> availableCommands;
    availableCommands.emplace_back("attack - attack the opponent cell field"); 
    availableCommands.emplace_back("pause - pause game");
    availableCommands.emplace_back("finish - finish the game");
    ViewHelper::consoleOut("Availabale Commands");
    for (int i = 0;i < availableCommands.size();i++){
        ViewHelper::consoleOut(std::to_string(i + 1) + ") " + availableCommands[i]);
    }
}

void OngoingGameView::printGameRules() {
    std::string rules = "1. PlaceShip\n";
    //TODO: split the rules on Chapters for example 1.PlaceShip all rules about placeShip 2.Battel and etc.
}