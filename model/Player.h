#pragma once
#include "AttackHandler.h"
#include "ParserCommandInfo.h"
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
    bool makeAShot(ParsedOptions options);
    void applySkill(ParsedOptions options);
};
