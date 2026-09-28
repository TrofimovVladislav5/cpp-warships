#pragma once

#include <application/head/common/ScreenKind.h>
#include <application/head/common/ThemeSelection.h>
#include <application/head/common/input/GridGeometry.h>
#include <application/head/common/state/PresentationState.h>
#include <application/model/WarshipsGame.h>

namespace cpp_warships::model {
    class ApplicationContext;
}

namespace cpp_warships::head::common {
    /** @brief Everything the interface works from, in one place: the game as something to read
     * and never to change, and all of the interface's own remembering beside it. */
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

        [[nodiscard]] state::PresentationState& state() noexcept;
        [[nodiscard]] const state::PresentationState& state() const noexcept;

        [[nodiscard]] input::GridGeometry& geometry() noexcept;
        [[nodiscard]] const input::GridGeometry& geometry() const noexcept;
        /** @brief Which screen should be showing. */
        [[nodiscard]] ScreenKind currentScreen() const;

    private:
        const model::ApplicationContext& application_;
        ThemeSelection theme_;
        state::PresentationState state_;
        input::GridGeometry geometry_;
    };
}  // namespace cpp_warships::head::common
