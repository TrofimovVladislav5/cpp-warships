#pragma once
#include "GameField.h"
#include "Skill.h"

class DoubleDamageSkill : public ISkill{
private:
	std::pair<int, int> attackCoordinate;
	GameField* gameField;
public:
	DoubleDamageSkill(std::pair<int, int> coord, GameField* gameField);
	void apply() override;
};

