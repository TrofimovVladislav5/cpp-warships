#pragma once

#include <application/core/Coordinate.h>
#include <application/flow/RandomEngine.h>
#include <application/flow/SkillKind.h>

#include <optional>

namespace cpp_warships::core {
    class Board;
}

namespace cpp_warships::flow {
    struct MatchEvent;
}

namespace cpp_warships::flow {
    /** @brief The primitives a skill may use against the match applying it.
     * Deliberately dumb: what a skill does with these is the skill's own business. */
    class SkillContext {
    public:
        virtual ~SkillContext() = default;

        /** @brief The board the skill acts against. */
        [[nodiscard]] virtual const core::Board& enemyBoard() const = 0;

        /** @brief The match's random source, so a skill's choices replay from the seed. */
        [[nodiscard]] virtual RandomEngine& randomEngine() = 0;

        /** @brief Fires at @p coordinate on the player's behalf, with the usual consequences bar
         * one: it does not hand the turn over. */
        virtual void strikeEnemyCell(core::Coordinate coordinate) = 0;

        /** @brief Arms the next shot to deal double damage. */
        virtual void armDoubleDamage() = 0;

        /** @brief Appends an event to the match history. */
        virtual void recordSkillEvent(const MatchEvent& event) = 0;
    };

    /** @brief What applying one kind of skill does. One implementation per skill,
     * so each skill's rules live in a single place. */
    class SkillBehaviour {
    public:
        virtual ~SkillBehaviour() = default;

        /** @brief Whether the player must supply a target cell before this can be applied. */
        [[nodiscard]] virtual bool needsTarget() const = 0;

        /** @brief Applies the skill; @p target is present exactly when needsTarget() holds. */
        virtual void apply(SkillContext& context, std::optional<core::Coordinate> target) const = 0;
    };

    /** @brief The behaviour describing @p skill. Returns a shared, stateless instance;
     * never null. */
    const SkillBehaviour& behaviourFor(SkillKind skill);
}  // namespace cpp_warships::flow
