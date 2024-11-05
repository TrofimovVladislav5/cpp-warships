#include "Player.h"

#include "SkillException.h"
#include "TypesHelper.h"

Player::Player(MatchSettings *settings)
    :   settings(settings)
{
    skillsManager = new SkillManager(settings);
}

Player::~Player() {
    delete skillsManager;
}

bool Player::makeATurn(ParsedOptions options) {
    if (!options["type"].empty()) {
        applySkill(options);
        return false;
    }
    return makeAShot(options);
}

void Player::applySkill(ParsedOptions options) {
    //TODO: parser command has or not replace options on command
    if (skillsManager->canUseSkill() != options["type"]) {
        throw SkillException("(where apply skill) Trying use unavailable skill " + options["type"] + " not equal" + skillsManager->canUseSkill());
    }
    if (options["type"] == "Scanner") {
        settings->setScanCoordinates(TypesHelper::cell(options["cell"]));
    }
    try {
        skillsManager->applySkill();
    }
    catch(const SkillException& exception) {
        exception.displayError();
    }
}

bool Player::makeAShot(ParsedOptions options) {
    AttackResult currentAttack = settings->getOpponentField()->attack(TypesHelper::cell(options["cell"]), settings->getDamage());
    if (settings->getIsActiveDoubleDamage()) {
        settings->setActiveDoubleDamage(false);
        settings->setDamageCount(1);
    }
    if (currentAttack != AttackResult::miss) {
        if (currentAttack == AttackResult::destroyed) {
            skillsManager->addSkill();
        }
        return true;
    }
    return false;
}




