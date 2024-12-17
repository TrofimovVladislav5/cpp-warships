#include "BattleController.h"
#include <random>

#include "exceptions/BattleException.h"
#include "exceptions/SkillException.h"

BattleController::BattleController(GameStateDTO* dto)
    : battleIsFinished(false)
    , player(new Player(dto))
    , playerView(new GameFieldView(dto->playerField))
    , opponentView(new GameFieldView(dto->enemyField))
    , computer(new ComputerPlayer(dto->playerField))
    , command("")
{
    skillManagerView = new SkillManagerView(dto->playerSkillManager);
}

BattleController::~BattleController() {
    delete playerView;
    delete opponentView;
    delete player;
    delete skillManagerView;
    delete computer;
}

bool BattleController::finishBattle() {
    command = player->isWin() ? "player" : "";
    command = computer->isWin() ? "computer" : "";
    return player->isWin() || computer->isWin();
}

void BattleController::applySkill(ParsedOptions options) {
    try {
        player->applySkill(options);
    }
    catch (const SkillException& exception) {
        exception.displayError();
    }

    computer->makeAShot();
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
}

void BattleController::pause(ParsedOptions options) {
    this->command = "pause";
}

std::string BattleController::getCommand() const {
    return command;
}