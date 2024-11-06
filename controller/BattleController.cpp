#include "BattleController.h"
#include "../library/ViewHelper.h"
#include "../library/defaults/BattleException.h"
#include <random>

#include "../library/defaults/SkillException.h"
#include "game-states/GameState.h"

BattleController::BattleController(MatchSettings& settings)
    : settings(settings)
    , battleIsFinished(false)
{
    playerView = new GameFieldView(settings.getPlayerField());
    opponentView = new GameFieldView(settings.getOpponentField());
    player = new Player(&settings);
    computer = new ComputerPlayer(settings.getPlayerField());
}

BattleController::~BattleController() {
    delete playerView;
    delete opponentView;
    delete player;
    delete computer;
}

bool BattleController::battleStatus() {
    GameField* playerField = settings.getPlayerField();
    GameField* opponentField = settings.getOpponentField();

    if (playerField->allShipsDestroyed()) {
        battleIsFinished = true;
        return true;
    } else if (opponentField->allShipsDestroyed()) {
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
        opponentView->displayField(true);
    }
}