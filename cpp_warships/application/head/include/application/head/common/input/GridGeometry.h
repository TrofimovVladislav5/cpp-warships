#pragma once

#include <application/core/Coordinate.h>

#include <optional>

namespace cpp_warships::head::common::input {
    /** @brief A part of the screen a pointer can be over, named in the game's
     * terms. */
    enum class ScreenRegion {
        Elsewhere,
        OwnWaters,
        EnemyWaters,
        Log,
    };

    /** @brief Where things landed the last time they were drawn, so that a
     * pointer position can be turned back into a cell. */
    class GridGeometry {
    public:
        /** @brief Notes that @p region was drawn as a @p boardWidth by @p boardHeight grid
         * filling the patch from (@p left, @p top) across @p width and down @p height. */
        void rememberBoard(
            ScreenRegion region,
            int left,
            int top,
            int width,
            int height,
            int boardWidth,
            int boardHeight,
            int columnPitch,
            int rowPitch
        );

        /** @brief Notes the patch the log was drawn into. */
        void rememberLog(int left, int top, int width, int height);

        /** @brief Forgets everything, so that a region not drawn this time is
         * not still answering for where it was last time. */
        void clear() noexcept;

        /** @brief Which part of the screen (@p screenX, @p screenY) is over. */
        [[nodiscard]] ScreenRegion regionAt(int screenX, int screenY) const;

        /** @brief The cell of @p region under (@p screenX, @p screenY), if it
         * is over it. */
        [[nodiscard]] std::optional<core::Coordinate> cellAt(
            ScreenRegion region,
            int screenX,
            int screenY
        ) const;

    private:
        /** @brief One drawn patch, and the grid that was drawn into it. */
        struct Patch {
            bool isKnown = false;
            int left = 0;
            int top = 0;
            int width = 0;
            int height = 0;
            int boardWidth = 0;
            int boardHeight = 0;
            int columnPitch = 1;
            int rowPitch = 1;

            [[nodiscard]] bool contains(int screenX, int screenY) const;
            [[nodiscard]] std::optional<core::Coordinate> cellAt(int screenX, int screenY) const;
        };

        [[nodiscard]] const Patch& patchFor(ScreenRegion region) const;
        [[nodiscard]] Patch& patchFor(ScreenRegion region);

        Patch ownWaters_;
        Patch enemyWaters_;
        Patch log_;
        Patch elsewhere_;
    };
}  // namespace cpp_warships::head::common::input
