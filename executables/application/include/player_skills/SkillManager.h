#pragma once

#include <queue>
#include <string>

#include "Skill.h"
#include "SkillFactory.h"
#include "../GameField.h"
#include "../MatchSettings.h"

namespace cpp_warships::application {
    inline char SkillManagerName[] = "SkillManager";

    class SkillManager
        : public game_saves::ISerializable<SkillManagerName>
    {
    private:
        GameField* referenceField;
        MatchSettings* matchSettings;
        std::deque<std::string> skills;
        std::vector<std::string> availableSkills;
        std::unordered_map<std::string, SkillFactory*> factory;
        ISkill* currentSkill;
        ISkill* createSkill(const std::string& skillName);
        void randomSkill();
    public:
        explicit SkillManager(GameField* enemyField, MatchSettings* settings);
        explicit SkillManager(const std::deque<std::string>& skills, GameField* enemyField, MatchSettings* settings);
        ~SkillManager() override;

        [[nodiscard]] GameField* getReferenceField() const;
        [[nodiscard]] MatchSettings* getMatchSettings() const;

        const std::vector<std::string>& nameSkills();
        std::string availableSkill();
        void addSkill();
        void applySkill();
        void status() const;
        void setSkills(const std::deque<std::string>& dequeSkills);
        [[nodiscard]] std::deque<std::string> getSkillsQueue() const;
    };
} // namespace cpp_warships::application

