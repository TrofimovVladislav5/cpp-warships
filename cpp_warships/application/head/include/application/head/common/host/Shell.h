#pragma once

#include <functional>

namespace cpp_warships::head {
    class EventPipeline;
    class PresentationContext;
    class RendererSet;

    /** @brief Whether the session has been asked to end. */
    using SessionFinishedQuery = std::function<bool()>;

    /** @brief Whatever is hosting the interface: something that draws what @p
     * context currently looks like and feeds back what the player does. */
    class Shell {
       public:
        virtual ~Shell() = default;

        /** @brief Draws @p context through @p renderers and feeds what the
         * player does into @p pipeline, until @p isFinished says the session is
         * over. */
        virtual void run(
            PresentationContext& context,
            RendererSet& renderers,
            EventPipeline& pipeline,
            const SessionFinishedQuery& isFinished
        ) = 0;
    };
}  // namespace cpp_warships::head
