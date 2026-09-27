#pragma once

#include <application/head/ThemeSelection.h>
#include <application/head/input/GridGeometry.h>
#include <application/head/screens/ScreenKind.h>
#include <application/head/state/PresentationState.h>
#include <application/model/ApplicationContext.h>

namespace cpp_warships::head {
    /** @brief Everything the interface works from, in one place: the game as something to
     *  read and never to change, and all of the interface's own remembering beside it.
     *  The game arrives const by construction, so no renderer and no reader of input has
     *  any way to alter it. Asking for a change is what events are for. */
    class PresentationContext {
    public:
        /** @brief Presents @p application, which must outlive it. */
        explicit PresentationContext(const model::ApplicationContext& application) noexcept;

        /** @brief The game, to read. */
        [[nodiscard]] const model::ApplicationContext& application() const noexcept;

        /** @brief The game being played, as a shorthand for what renderers ask most. */
        [[nodiscard]] const model::WarshipsGame& game() const noexcept;

        [[nodiscard]] const Theme& theme() const noexcept;
        [[nodiscard]] ThemeSelection& themeSelection() noexcept;

        [[nodiscard]] PresentationState& state() noexcept;
        [[nodiscard]] const PresentationState& state() const noexcept;

        [[nodiscard]] GridGeometry& geometry() noexcept;
        [[nodiscard]] const GridGeometry& geometry() const noexcept;

        /** @brief Which screen should be showing. Worked out from where the game stands
         *  rather than asked for, so a phase changing is the whole of the navigation. */
        [[nodiscard]] ScreenKind currentScreen() const;

    private:
        const model::ApplicationContext& application_;
        ThemeSelection theme_;
        PresentationState state_;
        GridGeometry geometry_;
    };
} // namespace cpp_warships::head
