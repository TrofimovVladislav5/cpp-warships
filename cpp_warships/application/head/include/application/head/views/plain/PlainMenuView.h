#pragma once

#include <application/head/Queries.h>
#include <application/head/Theme.h>
#include <application/head/state/MenuState.h>
#include <application/head/PresentationContext.h>
#include <application/head/views/Renderer.h>

namespace cpp_warships::head {
    /** @brief The menu as the console game used to print it: a few lines, no colour.
     *  Reads what it was given and returns text; it changes nothing. */
    class PlainMenuView final : public Renderer {
    public:
        explicit PlainMenuView(const PresentationContext& context) noexcept;

        [[nodiscard]] Frame render(int availableWidth, int availableHeight) override;


    private:
        const PresentationContext& context_;
    };
} // namespace cpp_warships::head
