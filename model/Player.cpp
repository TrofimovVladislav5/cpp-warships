#include "Player.h"

#include "Structures.h"
#include "TypesHelper.h"
#include "game/GameStateDTO.h"

Player::Player(GameStateDTO* dto)
    : opponentField(dto->enemyField)
    , playerAttackHandler(new AttackHandler(opponentField, dto->settings))
    , skillsManager(nullptr)
{
    // skillsManager = new SkillManager(settings);
}

Player::~Player() {
    delete skillsManager;
}

bool Player::makeATurn(ParsedOptions options) {
    // if (!options["type"].empty()) {
    //     applySkill(options);
    //     return false;
    // }
    return makeAShot(options);
}

void Player::applySkill(ParsedOptions options) {
    // if (skillsManager->canUseSkill() != options["type"]) {
    //     throw SkillException("(where apply skill) Trying use unavailable skill " + options["type"] + " not equal" + skillsManager->canUseSkill());
    // }
    // if (options["type"] == "Scanner") {
    //     // settings->setScanCoordinates(TypesHelper::cell(options["cell"]));
    // }
    // try {
    //     skillsManager->applySkill();
    // }
    // catch(const SkillException& exception) {
    //     exception.displayError();
    // }
}

bool Player::makeAShot(ParsedOptions options) {
    std::pair<int, int> attackCell = TypesHelper::cell(options["cell"]);
    AttackResult currentAttack = playerAttackHandler->attack(attackCell);

    if (currentAttack != AttackResult::miss) {
        if (currentAttack == AttackResult::destroyed) {
            skillsManager->addSkill();
        }
        return true;
    }

    return false;
}