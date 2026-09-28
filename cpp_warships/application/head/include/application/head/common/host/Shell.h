#pragma once

#include <functional>

namespace cpp_warships::head::common {
    class PresentationContext;
}

namespace cpp_warships::head::common::input {
    class EventPipeline;
}

namespace cpp_warships::head::common::render {
    class RendererSet;
}

namespace cpp_warships::head::common::host {
    /** @brief Whether the session has been asked to end. */
    using SessionFinishedQuery = std::function<bool()>;

    /** @brief Whatever is hosting the interface: something that draws what the context
     * currently looks like and feeds back what the player does. */
    class Shell {
    public:
        virtual ~Shell() = default;

        /** @brief Draws @p context through @p renderers and feeds what the player does
         * into @p pipeline, until @p isFinished says the session is over. */
        virtual void run(
            PresentationContext& context,
            render::RendererSet& renderers,
            input::EventPipeline& pipeline,
            const SessionFinishedQuery& isFinished
        ) = 0;
    };
}  // namespace cpp_warships::head::common::host
