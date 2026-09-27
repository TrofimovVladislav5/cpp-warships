#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/state/PlacementState.h>
#include <application/head/PresentationContext.h>
#include <application/head/views/Renderer.h>

namespace cpp_warships::head {
    /** @brief The fleet being laid out, printed as the console game used to print it:
     *  the board ruled out in ASCII, the roster under it as a list. */
    class PlainPlacementView final : public Renderer {
    public:
        explicit PlainPlacementView(const PresentationContext& context) noexcept;

        [[nodiscard]] Frame render(int availableWidth, int availableHeight) override;


    private:
        const PresentationContext& context_;
    };
} // namespace cpp_warships::head
