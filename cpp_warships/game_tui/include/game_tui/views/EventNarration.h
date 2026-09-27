#pragma once

#include <string>

#include <game_tui/Color.h>

#include <game_flow/MatchEvent.h>
#include <game_flow/SkillKind.h>
#include <game_tui/Theme.h>

namespace cpp_warships::game_tui {
    /** @brief One line of the battle log: what happened, in the colour it deserves. */
    struct EventLine {
        std::string text;
        Color color;
    };

    /** @brief How @p event reads in the log under @p theme. */
    [[nodiscard]] EventLine narrate(const game_flow::MatchEvent& event, const Theme& theme);

    /** @brief What a skill is called on screen, which is not what a save file calls it. */
    [[nodiscard]] std::string skillName(game_flow::SkillKind skill);
} // namespace cpp_warships::game_tui
