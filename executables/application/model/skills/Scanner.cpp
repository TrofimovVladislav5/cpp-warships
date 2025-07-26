#include "Scanner.h"

#include <iostream>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/utilities/include/ViewHelper.h>

#include "../../view/GameFieldView.h"
#include "../exceptions/SkillException.h"

Scanner::Scanner(GameField* opponentField) : opponentField(opponentField) {}

void Scanner::apply() {
    try {
        std::string input;
        ViewHelper::consoleOut("Enter cell to scan area 2x2: ");
        std::getline(std::cin, input);
        std::pair<int, int> cell = TypesHelper::cell(input);
        bool contacting = opponentField->intersectsWithArea(cell, 1);
        ViewHelper::consoleOut(contacting ? "Ship in area" : "No ship in area");
    } catch (const std::exception& exception) {
        throw SkillException("Wrong cell value");
    }
}
