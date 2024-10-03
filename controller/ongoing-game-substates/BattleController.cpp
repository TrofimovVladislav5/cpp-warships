#include "model/StateContext.h"
#include "BattleController.h"
#include "view/ViewHelper.h"
#include "parser/Parser.h"

BattleController::BattleController(StateContext& context) 
    : context(context)
{}

bool BattleController::attackByPlayer(){
    Parser* parser = new Parser();
    ViewHelper::consoleOut("Choose cell on field to attack");
    // ViewEnemyField with Unknown cells

    std::pair<int, int> coordinateToAttack = parser->parseCoordinateCell();
    MatchSettings* settings = context.currentMatch->getSettings();
    GameField* gameField = settings->getPlayerField();
    
}