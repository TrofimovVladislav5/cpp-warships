#include "BattleController.h"
#include <random>

BattleController::BattleController(GameStateDTO* dto)
    : battleIsFinished(false)
    , player(new Player(dto))
    , playerView(new GameFieldView(dto->playerField))
    , opponentView(new GameFieldView(dto->enemyField))
    , computer(new ComputerPlayer(dto->playerField))
{}

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
    // std::vector<std::string> availableSkills = {
    //     {"Scanner"},
    //     {"DoubleDamage"},
    //     {"Shooting"}
    // };
    // if (std::find(availableSkills.begin(),availableSkills.end(),options["type"]) == availableSkills.end()) {
    //     throw BattleException("(where applySkill) Trying use unknown --type skill");
    // }
    // if (options["type"] == "Scanner") {
    //     if (options["cell"].empty()) {
    //         throw BattleException("(where applySkill) Trying use skill scanner without flag --cell <x,y>");
    //     }
    // }
    // try {
    //     player->applySkill(options);
    // }
    // catch (const SkillException& exception) {
    //     exception.displayError();
    // }
    computer->makeAShot();
}

void BattleController::battle(ParsedOptions options) {
    bool keepTurn = player->makeATurn(std::move(options));
    if (!keepTurn) {
        while (computer->makeAShot());
    }
    playerView->displayField(false, false);
    opponentView->displayField(true);
}