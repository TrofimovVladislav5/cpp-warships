#include "SkillManager.h"
#include "DoubleDamageSkill.h"
#include "ShootingRandomlySkill.h"
#include "Scanner.h"
#include "SkillFactory.h"
#include <cstdlib>
#include <ctime>
#include <set>

SkillManager::SkillManager(MatchSettings* settings) {
    std::srand(static_cast<unsigned int>(std::time(0)));

    ConcreteSkillFactory<Scanner, std::pair<int, int>, MatchSettings*> scannerFactory({0,0}, settings);

    ConcreteSkillFactory<DoubleDamageSkill, std::pair<int, int>, GameField*> doubleDamageFactory(
        {2, 3}, settings->getPlayerField());

    ConcreteSkillFactory<ShootingRandomlySkill, ShipManager*> shootingRandomlyFactory(settings->getPlayerManager());

    int skillType = std::rand() % 3;
    if (skillType == 0) {
        skills.push(scannerFactory.createSkill());
    } else if (skillType == 1){
        skills.push(doubleDamageFactory.createSkill());
    }
    else {
        skills.push(shootingRandomlyFactory.createSkill());
    }
}

void SkillManager::applySkills() {
    if (!skills.empty()) {
        skills.front()->apply();
        delete skills.front();
        skills.pop();
    } else {
        std::cout << "EXCEPTION" << std::endl; //TODO Exception
    }
}

SkillManager::~SkillManager() {
    while (!skills.empty()) {
        delete skills.front();
        skills.pop();
    }
}
