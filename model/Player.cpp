#include "Player.h"

#include "Structures.h"
#include "TypesHelper.h"
#include "exceptions/BattleException.h"
#include "exceptions/SkillException.h"
#include "game/GameStateDTO.h"

Player::Player(GameStateDTO* dto)
    : opponentField(dto->enemyField)
    , playerAttackHandler(new AttackHandler(opponentField, dto->settings))
    , skillsManager(dto->playerSkillManager)
{}

Player::~Player() {
    delete skillsManager;
}

void Player::applySkill(ParsedOptions options) {
    try {
        skillsManager->applySkill();
    }
    catch(const SkillException& exception) {
        exception.displayError();
    }
}

bool Player::makeAShot(ParsedOptions options) {

    std::pair<int, int> attackCell = TypesHelper::cell(options["cell"]);
    AttackResult currentAttack = playerAttackHandler->attack(attackCell);
    if (currentAttack == AttackResult::outOfBounds) {
        throw BattleException("Invalid cell coordinates");
    }
    if (currentAttack != AttackResult::miss) {
        if (currentAttack == AttackResult::destroyed) {
            skillsManager->addSkill();
        }
        return true;
    }

    return false;
}