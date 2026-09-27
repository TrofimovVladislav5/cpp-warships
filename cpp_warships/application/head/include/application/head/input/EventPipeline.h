#pragma once

#include <application/head/input/EventBus.h>
#include <application/head/input/Keystroke.h>
#include <application/head/PresentationContext.h>
#include <application/model/events/EventQueue.h>
#include <application/model/events/EventRouter.h>
#include <application/model/scenarios/ScenarioQueue.h>

namespace cpp_warships::head {
    /** @brief One turn of the whole machine: read what the player did, queue what they
     *  asked for, act on all of it in order, then show whatever the game now looks like.
     *  A host drives this and needs to know nothing else about how any of it works. */
    class EventPipeline {
    public:
        /** @brief Runs @p bus into @p events and out through @p router into @p scenarios,
         *  reading each stroke as the player being wherever @p context says they are.
         *  Everything given here must outlive it. */
        EventPipeline(
                PresentationContext& context,
                EventBus& bus,
                model::EventQueue& events,
                model::EventRouter& router,
                model::ScenarioQueue& scenarios
        );

        /** @brief Reads @p stroke and queues whatever it asks of the game, if anything. */
        void offer(const Keystroke& stroke);

        /** @brief Acts on everything queued, oldest first.
         *  @return whether anything at all came of it, which is what a host redraws for. */
        bool settle();

    private:
        PresentationContext& context_;
        EventBus& bus_;
        model::EventQueue& events_;
        model::EventRouter& router_;
        model::ScenarioQueue& scenarios_;
    };
} // namespace cpp_warships::head
