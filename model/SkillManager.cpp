#include "SkillManager.h"
#include "DoubleDamageSkill.h"
#include "ShootingRandomlySkill.h"
#include "Scanner.h"
#include "SkillFactory.h"
#include <cstdlib>
#include <ctime>

SkillManager::SkillManager(MatchSettings* settings) {
    std::srand(static_cast<unsigned int>(std::time(0)));

    // ConcreteSkillFactory<Scanner, std::pair<int, int>, std::pair<int, int>, GameFieldView&> scannerFactory(
    //     {0, 0}, {1, 1}, settings->getOpponentFieldView());

    ConcreteSkillFactory<DoubleDamageSkill, std::pair<int, int>, GameField*> doubleDamageFactory(
        {2, 3}, settings->getPlayerField());

    ConcreteSkillFactory<ShootingRandomlySkill, ShipManager*> shootingRandomlyFactory(settings->getOpponentManager());

    int skillType = std::rand() % 2;
    if (skillType == 0) {
        skills.push(doubleDamageFactory.createSkill());
    } else {
        skills.push(shootingRandomlyFactory.createSkill());
    }
}

void SkillManager::applySkills() {
    std::cout << skills.size() << std::endl;
    std::cout << typeid(skills.front()).name() << std::endl;
    if (!skills.empty()) {
        skills.front()->apply();
        delete skills.front();
        skills.pop();
    } else {
        std::cout << "Нет доступных навыков для применения." << std::endl;
    }
}

SkillManager::~SkillManager() {
    while (!skills.empty()) {
        delete skills.front();
        skills.pop();
    }
}
