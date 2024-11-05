#pragma once
#include "MatchSettings.h"
#include "Skill.h"

class DoubleDamage : public ISkill{
private:
	MatchSettings* settings;
public:
	explicit DoubleDamage(MatchSettings* settings);
	void apply() override;
};

