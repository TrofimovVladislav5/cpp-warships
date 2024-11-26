#include "SkillManagerView.h"

#include "ViewHelper.h"

SkillManagerView::SkillManagerView(SkillManager* skillManager)
    : skillManager(skillManager)
{}

void SkillManagerView::displayAvailableSkills() {
    std::deque<std::string> skills = skillManager->getSkillsQueue();
    ViewHelper::consoleOut("Available skills:");
    for (const auto& skill : skills) {
        ViewHelper::consoleOut(skill, 1);
    }
}

void SkillManagerView::displayCurrentSkill() {
    ViewHelper::consoleOut("Current skill on queue: " + skillManager->availableSkill());
}
