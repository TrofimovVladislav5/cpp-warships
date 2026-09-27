#pragma once

#include <application/core/Coordinate.h>
#include <application/head/Queries.h>
#include <application/head/input/EventHandler.h>
#include <application/head/intents/Intent.h>
#include <application/head/screens/PlacementState.h>

namespace cpp_warships::head {
    /** @brief Turns the ship in hand between lying across and lying down. */
    class RotateShipEventHandler final : public EventHandler {
    public:
        explicit RotateShipEventHandler(PlacementState& state);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        PlacementState& state_;
    };

    /** @brief Picks up the next length of ship still waiting to be placed. */
    class CycleShipLengthEventHandler final : public EventHandler {
    public:
        CycleShipLengthEventHandler(PlacementState& state, MatchQuery match);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        PlacementState& state_;
        MatchQuery match_;
    };

    /** @brief Lays the ship in hand at the cursor, leaving the board to refuse an illegal spot. */
    class PlaceShipEventHandler final : public EventHandler {
    public:
        PlaceShipEventHandler(IntentSink intentSink, const PlacementState& state, MatchQuery match);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
        const PlacementState& state_;
        MatchQuery match_;
    };

    /** @brief Takes back the ship lying under the cursor. */
    class RemoveShipEventHandler final : public EventHandler {
    public:
        RemoveShipEventHandler(IntentSink intentSink, const PlacementState& state);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
        const PlacementState& state_;
    };

    /** @brief Throws the whole fleet back onto the board at random. */
    class ShuffleFleetEventHandler final : public EventHandler {
    public:
        explicit ShuffleFleetEventHandler(IntentSink intentSink);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
    };

    /** @brief Opens fire, and only offers itself once every ship is on the board. */
    class BeginBattleEventHandler final : public EventHandler {
    public:
        BeginBattleEventHandler(IntentSink intentSink, MatchQuery match);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
        MatchQuery match_;
    };

    /** @brief Mouse work on the placement grid. Every gesture first takes aim at the cell
     *  under the pointer; what it does beyond that is the subclass's business. */
    class GridMouseEventHandler : public EventHandler {
    public:
        explicit GridMouseEventHandler(PlacementState& state);

        [[nodiscard]] bool isHandled(const InputEvent& input) const final;
        void handleEvent(const InputEvent& input) final;

    protected:
        /** @brief Whether this handler answers to @p mouse. */
        [[nodiscard]] virtual bool claims(const Keystroke& mouse) const = 0;

        /** @brief What the gesture does at @p cell, over and above having aimed there. */
        virtual void actOn(core::Coordinate cell) = 0;

        [[nodiscard]] PlacementState& state() const noexcept;

    private:
        PlacementState& state_;
    };

    /** @brief Follows the pointer around the grid, which is all a bare mouse move means. */
    class TakeAimWithMouseEventHandler final : public GridMouseEventHandler {
    public:
        using GridMouseEventHandler::GridMouseEventHandler;

    protected:
        [[nodiscard]] bool claims(const Keystroke& mouse) const override;
        void actOn(core::Coordinate cell) override;
    };

    /** @brief Lays the ship in hand where the left button goes down. */
    class LayShipWithMouseEventHandler final : public GridMouseEventHandler {
    public:
        LayShipWithMouseEventHandler(
                IntentSink intentSink,
                PlacementState& state,
                MatchQuery match
        );

    protected:
        [[nodiscard]] bool claims(const Keystroke& mouse) const override;
        void actOn(core::Coordinate cell) override;

    private:
        IntentSink intentSink_;
        MatchQuery match_;
    };

    /** @brief Takes back the ship under the right button. */
    class TakeBackShipWithMouseEventHandler final : public GridMouseEventHandler {
    public:
        TakeBackShipWithMouseEventHandler(IntentSink intentSink, PlacementState& state);

    protected:
        [[nodiscard]] bool claims(const Keystroke& mouse) const override;
        void actOn(core::Coordinate cell) override;

    private:
        IntentSink intentSink_;
    };

    /** @brief Rolls the wheel through the lengths still waiting to be placed. */
    class PickShipWithWheelEventHandler final : public GridMouseEventHandler {
    public:
        PickShipWithWheelEventHandler(PlacementState& state, MatchQuery match);

    protected:
        [[nodiscard]] bool claims(const Keystroke& mouse) const override;
        void actOn(core::Coordinate cell) override;

    private:
        MatchQuery match_;
    };

    /** @brief Goes back to the menu, leaving the fleet as it stands. */
    class LeavePlacementEventHandler final : public EventHandler {
    public:
        explicit LeavePlacementEventHandler(IntentSink intentSink);

        [[nodiscard]] bool isHandled(const InputEvent& input) const override;
        void handleEvent(const InputEvent& input) override;

    private:
        IntentSink intentSink_;
    };
} // namespace cpp_warships::head
