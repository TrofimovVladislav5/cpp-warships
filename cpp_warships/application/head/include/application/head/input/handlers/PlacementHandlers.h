#pragma once

#include <application/head/Queries.h>
#include <application/head/input/handlers/SessionHandlers.h>
#include <application/model/events/EventHandler.h>

namespace cpp_warships::head {
    /** @brief Lays a ship where the player asked for one. */
    class PlaceShipHandler final : public model::EventHandler {
    public:
        explicit PlaceShipHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

    private:
        HandlerParts parts_;
    };

    /** @brief Takes a ship back off the board. */
    class RemoveShipHandler final : public model::EventHandler {
    public:
        explicit RemoveShipHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

    private:
        HandlerParts parts_;
    };

    /** @brief Lays the whole fleet out at random. */
    class ShuffleFleetHandler final : public model::EventHandler {
    public:
        explicit ShuffleFleetHandler(HandlerParts parts) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

    private:
        HandlerParts parts_;
    };

    /** @brief Opens fire, but only once every ship has somewhere to be.
     *  That is a rule, so it is asked here rather than by whatever read the key. */
    class BeginBattleHandler final : public model::EventHandler {
    public:
        BeginBattleHandler(HandlerParts parts, MatchQuery match) noexcept;

        [[nodiscard]] bool isHandled(const model::GameEvent& event) const override;
        void handleEvent(const model::GameEvent& event) override;

    private:
        HandlerParts parts_;
        MatchQuery match_;
    };
} // namespace cpp_warships::head
