#include "BattleController.h"
#include <random>

#include "ViewHelper.h"
#include "exceptions/BattleException.h"
#include "exceptions/SkillException.h"


void BattleController::printBattleState() {
    playerView->displayField(false, false);
    opponentView->displayField(false, false);
    skillManagerView->displayCurrentSkill();
    skillManagerView->displayAvailableSkills();
}

BattleController::BattleController(GameStateDTO* dto)
    : battleIsFinished(false)
    , player(new Player(dto))
    , playerView(new GameFieldView(dto->playerField))
    , opponentView(new GameFieldView(dto->enemyField))
    , computer(new ComputerPlayer(dto->playerField))
    , saveCreator(new GameSaveCreator(dto))
{
    skillManagerView = new SkillManagerView(dto->playerSkillManager);

}

BattleController::~BattleController() {
    delete playerView;
    delete opponentView;
    delete player;
    delete skillManagerView;
    delete computer;
    delete saveCreator;
}

bool BattleController::finishBattle() const{
    return battleIsFinished;
}

void BattleController::applySkill(ParsedOptions options) {
    try {
        player->applySkill(options);
    }
    catch (const SkillException& exception) {
        exception.displayError();
    }

    computer->makeAShot();
    printBattleState();
}

void BattleController::battle(ParsedOptions options) {
    try {
        bool keepTurn = player->makeAShot(std::move(options));

        if (!keepTurn) {
            while (computer->makeAShot());
        }
    } catch (const BattleException& exception) {
        exception.displayError();
    }

    printBattleState();
}

void BattleController::pause(ParsedOptions options) {
    this->commandPause = "pause";
}

std::string BattleController::getPause() const {
    return commandPause;
}