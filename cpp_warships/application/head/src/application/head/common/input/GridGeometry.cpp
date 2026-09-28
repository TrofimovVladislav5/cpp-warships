#include <application/head/common/input/GridGeometry.h>

namespace cpp_warships::head {
    bool GridGeometry::Patch::contains(const int screenX, const int screenY) const {
        return isKnown && screenX >= left && screenX < left + width && screenY >= top &&
               screenY < top + height;
    }

    std::optional<core::Coordinate>
    GridGeometry::Patch::cellAt(const int screenX, const int screenY) const {
        if (!contains(screenX, screenY) || columnPitch <= 0 || rowPitch <= 0) {
            return std::nullopt;
        }

        const int column = (screenX - left) / columnPitch;
        const int row = (screenY - top) / rowPitch;
        if (column < 0 || column >= boardWidth || row < 0 || row >= boardHeight) {
            return std::nullopt;
        }

        return core::Coordinate{column, row};
    }

    void GridGeometry::rememberBoard(
        const ScreenRegion region,
        const int left,
        const int top,
        const int width,
        const int height,
        const int boardWidth,
        const int boardHeight,
        const int columnPitch,
        const int rowPitch
    ) {
        patchFor(region) = Patch{
            .isKnown = true,
            .left = left,
            .top = top,
            .width = width,
            .height = height,
            .boardWidth = boardWidth,
            .boardHeight = boardHeight,
            .columnPitch = columnPitch,
            .rowPitch = rowPitch
        };
    }

    void
    GridGeometry::rememberLog(const int left, const int top, const int width, const int height) {
        log_ = Patch{
            .isKnown = true,
            .left = left,
            .top = top,
            .width = width,
            .height = height,
            .boardWidth = 0,
            .boardHeight = 0,
            .columnPitch = 1,
            .rowPitch = 1
        };
    }

    void GridGeometry::clear() noexcept {
        ownWaters_ = Patch{};
        enemyWaters_ = Patch{};
        log_ = Patch{};
    }

    ScreenRegion GridGeometry::regionAt(const int screenX, const int screenY) const {
        if (enemyWaters_.contains(screenX, screenY)) {
            return ScreenRegion::EnemyWaters;
        }
        if (ownWaters_.contains(screenX, screenY)) {
            return ScreenRegion::OwnWaters;
        }
        if (log_.contains(screenX, screenY)) {
            return ScreenRegion::Log;
        }

        return ScreenRegion::Elsewhere;
    }

    std::optional<core::Coordinate>
    GridGeometry::cellAt(const ScreenRegion region, const int screenX, const int screenY) const {
        return patchFor(region).cellAt(screenX, screenY);
    }

    const GridGeometry::Patch& GridGeometry::patchFor(const ScreenRegion region) const {
        switch (region) {
            case ScreenRegion::OwnWaters:
                return ownWaters_;
            case ScreenRegion::EnemyWaters:
                return enemyWaters_;
            case ScreenRegion::Log:
                return log_;
            case ScreenRegion::Elsewhere:
                break;
        }

        return elsewhere_;
    }

    GridGeometry::Patch& GridGeometry::patchFor(const ScreenRegion region) {
        const GridGeometry& self = *this;
        return const_cast<Patch&>(self.patchFor(region));
    }
}  // namespace cpp_warships::head
