#pragma once
#include "ParserCommandInfo.h"
#include "game/GameField.h"
#include "game/GameStateDTO.h"
#include "game/MatchSettings.h"
#include "skills/SkillManager.h"

class Player {
private:
    GameField* opponentField;
    AttackHandler* playerAttackHandler;
    SkillManager* skillsManager;
public:
    explicit Player(GameStateDTO* dto);
    ~Player();
    bool makeATurn(ParsedOptions options);
    bool makeAShot(ParsedOptions options);
    void applySkill(ParsedOptions options);
};
