#pragma once
#include "Skill.h"
#include <tuple>

class SkillFactory {
public:
    virtual ISkill* createSkill() = 0;
    virtual ~SkillFactory() = default;
};

template <typename SkillType, typename... Args>
class ConcreteSkillFactory : public SkillFactory {
private:
    std::tuple<Args...> args;
public:
    explicit ConcreteSkillFactory(Args... b) : args(std::make_tuple(b...)) {}

    ISkill* createSkill() override {
        return std::apply([](Args... args){return new SkillType(args...);}, args);
    }
};
