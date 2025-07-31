#pragma once

#include "Skill.h"

namespace cpp_warships::application {

    class SkillFactory {
    public:
        virtual ISkill* createSkill() = 0;
        virtual ~SkillFactory() = default;
    };

    template <typename SkillType, typename... Args>
    class ConcreteSkillFactory final : public SkillFactory {
    private:
        std::tuple<Args...> args;

    public:
        explicit ConcreteSkillFactory(Args... args) : args(std::make_tuple(args...)) {}

        ISkill* createSkill() override {
            return std::apply([](Args... args) { return new SkillType(args...); }, args);
        }
    };
} // namespace cpp_warships::application