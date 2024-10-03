#include <iostream>
#include "Parser.h"
#include "view/ViewHelper.h"

bool Parser::parseNumber(std::string stringNumber) {
    std::cmatch groups;
    std::regex pattern("(-?\\d+)");
    if (!std::regex_match(stringNumber.c_str(), groups, pattern)) {
        ViewHelper::consoleOut("Invalid format of int number");
        return false;
    } 
    int number = std::stoi(groups[1]);
    if (number <= 0) {
        ViewHelper::consoleOut("Invalid format: negative digit");
        return false;
    }
    return true;
}

int Parser::parseLengthShip(){
    int length;
    bool lengthChoseCorrect = false;
    ViewHelper::consoleOut("Enter a length of ship from 1 to 4");
    while (!lengthChoseCorrect) {
        std::string potentialLength;
        potentialLength.clear();
        std::getline(std::cin, potentialLength);
        bool parseDigitSuccess = parseNumber(potentialLength);
        if (parseDigitSuccess){
            length = std::stoi(potentialLength);
            if (length < 1 || length > 4) {
                ViewHelper::consoleOut("length should be from 1 to 4");
                lengthChoseCorrect = false;
            }
            else {
                lengthChoseCorrect = true;
            }
        }
        else {
            lengthChoseCorrect = false;
        }
    }
    return length;
}

Direction Parser::parseDirectionShip(){
    ViewHelper::consoleOut("Enter a direction to place ship: horizontal or vertical");
    Direction direction;
    bool directionChoseCorrect = false;
    while (!directionChoseCorrect) {
        std::string potentialDirection;
        std::getline(std::cin, potentialDirection);
        if (!(potentialDirection == "horizontal" || potentialDirection == "vertical")) {
            ViewHelper::consoleOut("Invalid direction: Please enter 'horizontal' or 'vertical'");
            directionChoseCorrect = false;
        } 
        else {
            if (potentialDirection == "horizontal") {
                direction = Direction::HORIZONTAL;
            } else if (potentialDirection == "vertical") {
                direction = Direction::VERTICAL;
            }
            directionChoseCorrect = true;
        }
        potentialDirection.clear();
    }
    return direction;
}

std::pair<int, int> Parser::parseCoordinateCell() {
    ViewHelper::consoleOut("Enter a cell in format x y on GameField");
    std::pair<int, int> potentialCoordinateCell;
    bool parseCoordinateSuccess = false;
    
    while (!parseCoordinateSuccess) {
        std::string potentialCoordinate;
        std::getline(std::cin, potentialCoordinate);
        
        std::smatch groups;
        parseCoordinateSuccess = parseCoordinate(potentialCoordinate, groups);
        
        if (parseCoordinateSuccess) {
            potentialCoordinateCell = {std::stoi(groups[1].str()), std::stoi(groups[2].str())};
        }
    }
    
    return potentialCoordinateCell;
}

bool Parser::parseCoordinate(const std::string& stringCoordinates, std::smatch& groups) {
    std::regex pattern("(\\d+)\\s(\\d+)");
    if (!std::regex_match(stringCoordinates, groups, pattern)) {
        std::cout << "Invalid argument: Coordinates not matched format x y" << std::endl;
        return false;
    }
    
    return true;
}