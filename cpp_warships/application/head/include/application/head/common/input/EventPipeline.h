#pragma once

namespace cpp_warships::head::common::input {
    class EventBus;
    struct Keystroke;
}  // namespace cpp_warships::head::common::input

namespace cpp_warships::model::events {
    class EventQueue;
    class EventRouter;
}  // namespace cpp_warships::model::events

namespace cpp_warships::model::scenarios {
    class ScenarioQueue;
}

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input {
    /** @brief One turn of the whole machine: read what the player did, queue what they asked
     * for, act on all of it in order, then show whatever the game now looks like. */
    class EventPipeline {
    public:
        /** @brief Runs @p bus into @p events and out through @p router into @p scenarios,
         * reading each stroke as the player being wherever @p context says they are. */
        EventPipeline(
            PresentationContext& context,
            EventBus& bus,
            model::events::EventQueue& events,
            model::events::EventRouter& router,
            model::scenarios::ScenarioQueue& scenarios
        );

        /** @brief Reads @p stroke and queues whatever it asks of the game, if
         * anything. */
        void offer(const Keystroke& stroke);

        /** @brief Acts on everything queued, oldest first.          *  @return whether anything at
         * all came of it, which is what a host redraws for. */
        bool settle();

    private:
        PresentationContext& context_;
        EventBus& bus_;
        model::events::EventQueue& events_;
        model::events::EventRouter& router_;
        model::scenarios::ScenarioQueue& scenarios_;
    };
}  // namespace cpp_warships::head::common::input
