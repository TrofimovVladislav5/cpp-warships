#pragma once

#include <cpp_warships/input_parser/include/model/ParserCommandInfo.h>

#include "../controller/AttackHandler.h"
#include "game/GameField.h"
#include "game/GameStateDTO.h"
#include "skills/SkillManager.h"

class Player {
private:
    GameField* opponentField;
    AttackHandler* playerAttackHandler;
    SkillManager* skillsManager;

public:
    explicit Player(GameStateDTO* dto);
    ~Player();
    bool isWin() const;
    bool makeAShot(cpp_warships::input_parser::ParsedOptions options);
    void applySkill(cpp_warships::input_parser::ParsedOptions options);
};
