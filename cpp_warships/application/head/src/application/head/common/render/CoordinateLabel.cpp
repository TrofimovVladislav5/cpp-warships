#include <application/head/common/render/CoordinateLabel.h>

#include <cstddef>

namespace cpp_warships::head::common::render {
    namespace {
        const std::string COLUMN_LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }  // namespace

    std::string columnLabel(const int column) {
        if (column < 0 || column >= static_cast<int>(COLUMN_LETTERS.size())) {
            return "?";
        }

        return std::string(1, COLUMN_LETTERS[static_cast<std::size_t>(column)]);
    }

    std::string coordinateLabel(const core::Coordinate coordinate) {
        return columnLabel(coordinate.x) + std::to_string(coordinate.y + 1);
    }
}  // namespace cpp_warships::head::common::render
