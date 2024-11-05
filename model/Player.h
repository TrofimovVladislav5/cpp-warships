#pragma once
#include "ParserCommandInfo.h"
#include "SkillManager.h"

class Player {
private:
    MatchSettings* settings;
    SkillManager* skillsManager;
public:
    explicit Player(MatchSettings* settings);
    ~Player();
    bool makeATurn(ParsedOptions options);
    bool makeAShot(ParsedOptions options);
    void applySkill(ParsedOptions options);
};
