#pragma once

#include <application/flow/SkillKind.h>
#include <application/head/common/Color.h>

#include <string>

namespace cpp_warships::flow {
    struct MatchEvent;
}

namespace cpp_warships::head::common {
    struct Theme;
}

namespace cpp_warships::head::common::render {
    /** @brief One line of the battle log: what happened, in the colour it deserves. */
    struct EventLine {
        std::string text;
        Color color;
    };

    /** @brief How @p event reads in the log under @p theme. */
    [[nodiscard]] EventLine narrate(const flow::MatchEvent& event, const Theme& theme);

    /** @brief What a skill is called on screen, which is not what a save file
     * calls it. */
    [[nodiscard]] std::string skillName(flow::SkillKind skill);
}  // namespace cpp_warships::head::common::render
