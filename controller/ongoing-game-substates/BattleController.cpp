#include "model/StateContext.h"
#include "BattleController.h"
#include "../../library/ViewHelper.h"
#include "defaults/BattleException.h"
#include <random>

#include "SkillException.h"

BattleController::BattleController(StateContext& context) 
    :   context(context)
    ,   battleIsFinished(false)
{
    playerView = new GameFieldView(context.currentMatch->getSettings()->getPlayerField());
    opponentView = new GameFieldView(context.currentMatch->getSettings()->getOpponentField());
    player = new Player(context.currentMatch->getSettings());
    computer = new ComputerPlayer(context.currentMatch->getSettings()->getPlayerField());
}

BattleController::~BattleController() {
    delete playerView;
    delete opponentView;
    delete player;
    delete computer;
}

bool BattleController::battleStatus() {
    if (context.currentMatch->getSettings()->getPlayerField()->allShipsDestroyed()) {
        ViewHelper::consoleOut("Player lose game");
        battleIsFinished = true;
        return true;
    }
    if (context.currentMatch->getSettings()->getOpponentField()->allShipsDestroyed()) {
        ViewHelper::consoleOut("Player win game");
        battleIsFinished = true;
        return true;
    }
    return false;
}
bool BattleController::finishBattle() const{
    return battleIsFinished;
}

void BattleController::applySkill(ParsedOptions options) {
    std::vector<std::string> availableSkills = {
        {"Scanner"},
        {"DoubleDamage"},
        {"Shooting"}
    };
    if (std::find(availableSkills.begin(),availableSkills.end(),options["type"]) == availableSkills.end()) {
        throw BattleException("(where applySkill) Trying use unknown --type skill");
    }
    if (options["type"] == "Scanner") {
        if (options["cell"].empty()) {
            throw BattleException("(where applySkill) Trying use skill scanner without flag --cell <x,y>");
        }
    }
    try {
        player->applySkill(options);
    }
    catch (const SkillException& exception) {
        exception.displayError();
    }
    computer->makeAShot();
}

void BattleController::battle(ParsedOptions options) {
    if (!battleStatus()) {
        bool keepTurn = player->makeATurn(options);
        if (!keepTurn && !battleStatus()) {
            computer->makeAShot();
        }
        playerView->displayField(false);
        opponentView->displayField(false);
    }
}