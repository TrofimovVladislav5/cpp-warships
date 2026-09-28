#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/input/handlers/SessionHandlers.h>
#include <application/model/events/EventHandler.h>

namespace cpp_warships::head {
    /** @brief Fires on a cell, but only while it is the player's turn to. */
    class FireHandler final : public model::EventHandler {
       public:
        FireHandler(HandlerParts parts, MatchQuery match) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

       private:
        HandlerParts parts_;
        MatchQuery match_;
    };

    /** @brief Spends the next banked skill, if there is one and it is the
     * player's turn. */
    class UseSkillHandler final : public model::EventHandler {
       public:
        UseSkillHandler(HandlerParts parts, MatchQuery match) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

       private:
        HandlerParts parts_;
        MatchQuery match_;
    };
}  // namespace cpp_warships::head
