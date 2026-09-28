#pragma once

#include <application/head/common/Queries.h>
#include <application/head/common/input/handlers/SessionHandlers.h>
#include <application/model/events/EventHandler.h>

namespace cpp_warships::head::common::input::handlers {
    /** @brief Lays a ship where the player asked for one. */
    class PlaceShipHandler final : public model::events::EventHandler {
    public:
        explicit PlaceShipHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
    };

    /** @brief Takes a ship back off the board. */
    class RemoveShipHandler final : public model::events::EventHandler {
    public:
        explicit RemoveShipHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
    };

    /** @brief Lays the whole fleet out at random. */
    class ShuffleFleetHandler final : public model::events::EventHandler {
    public:
        explicit ShuffleFleetHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
    };

    /** @brief Opens fire, but only once every ship has somewhere to be. */
    class BeginBattleHandler final : public model::events::EventHandler {
    public:
        BeginBattleHandler(HandlerParts parts, MatchQuery match) noexcept;

        [[nodiscard]] bool isHandled(const model::events::GameEvent& event) const override;
        void handleEvent(const model::events::GameEvent& event) override;

    private:
        HandlerParts parts_;
        MatchQuery match_;
    };
}  // namespace cpp_warships::head::common::input::handlers
