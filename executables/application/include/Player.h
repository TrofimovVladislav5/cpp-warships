#pragma once

#include <cpp_warships/input_parser/include/model/ParserCommandInfo.h>

#include "AttackHandler.h"
#include "GameField.h"
#include "GameStateDTO.h"
#include "player_skills/SkillManager.h"

namespace cpp_warships::application {

    class Player {
    private:
        GameField* opponentField;
        AttackHandler* playerAttackHandler;
        SkillManager* skillsManager;

    public:
        explicit Player(GameStateDTO* dto);
        ~Player();
        bool isWin() const;
        bool makeAShot(input_parser::ParsedOptions options);
        void applySkill(input_parser::ParsedOptions options);
    };
} // namespace cpp_warships::application