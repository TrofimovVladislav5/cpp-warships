#include "BattleController.h"

#include <random>

#include "../model/exceptions/BattleException.h"
#include "../model/exceptions/SkillException.h"

BattleController::BattleController(GameStateDTO* dto)
    : battleIsFinished(false),
      player(new Player(dto)),
      playerView(new GameFieldView(dto->playerField)),
      opponentView(new GameFieldView(dto->enemyField)),
      computer(new ComputerPlayer(dto->playerField)) {
    skillManagerView = new SkillManagerView(dto->playerSkillManager);
}

BattleController::~BattleController() {
    delete playerView;
    delete opponentView;
    delete player;
    delete skillManagerView;
    delete computer;
}

void BattleController::applySkill(cpp_warships::input_parser::ParsedOptions options) {
    try {
        player->applySkill(options);
    } catch (const SkillException& exception) {
        exception.displayError();
    }

    computer->makeAShot();
}

void BattleController::battle(cpp_warships::input_parser::ParsedOptions options) {
    try {
        bool keepTurn = player->makeAShot(std::move(options));

        if (!keepTurn) {
            while (computer->makeAShot());
        }
    } catch (const BattleException& exception) {
        exception.displayError();
    }
}

BattleWinner BattleController::getBattleWinner() const {
    if (player->isWin())
        return BattleWinner::User;
    else if (computer->isWin())
        return BattleWinner::Computer;

    return BattleWinner::None;
}