#include "../include/Player.h"

#include <cpp_warships/utilities/include/TypesHelper.h>

#include "../include/Structures.h"
#include "../include/GameStateDTO.h"
#include "../include/exceptions/BattleException.h"
#include "../include/player_skills/exceptions/SkillException.h"

namespace cpp_warships::application {

    Player::Player(GameStateDTO* dto)
        : opponentField(dto->enemyField),
          playerAttackHandler(new AttackHandler(opponentField, dto->settings)),
          skillsManager(dto->playerSkillManager) {}

    Player::~Player() {
        delete playerAttackHandler;
        delete skillsManager;
        delete opponentField;
    }

    void Player::applySkill(input_parser::ParsedOptions options) {
        try {
            skillsManager->applySkill();
        } catch (const SkillException& exception) {
            exception.displayError();
        }
    }

    bool Player::isWin() const {
        return opponentField->isAllShipsDestroyed();
    }

    bool Player::makeAShot(input_parser::ParsedOptions options) {
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
} // namespace cpp_warships::application