#pragma once

namespace cpp_warships::application {

    class ISkill {
    public:
        virtual void apply() = 0;
    };
} // namespace cpp_warships::application