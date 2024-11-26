#include "BattleController.h"
#include <random>

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
{
    skillManagerView = new SkillManagerView(dto->playerSkillManager);

}

BattleController::~BattleController() {
    delete playerView;
    delete opponentView;
    delete player;
    delete computer;
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
    bool keepTurn = player->makeAShot(std::move(options));

    if (!keepTurn) {
        while (computer->makeAShot());
    }

    printBattleState();
}