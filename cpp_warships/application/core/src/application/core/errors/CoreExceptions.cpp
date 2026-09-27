#include <application/core/errors/CoreExceptions.h>

namespace cpp_warships::core {
    namespace {
        /** @brief A cell written out for a message. Numbers, not the A1 the interface shows:
         *  the rules do not know how anyone chooses to label a board. */
        [[nodiscard]] std::string describe(const Coordinate coordinate) {
            return "(" + std::to_string(coordinate.x) + ", " + std::to_string(coordinate.y) + ")";
        }

        [[nodiscard]] std::string describe(const Direction direction) {
            return direction == Direction::Horizontal ? "horizontal" : "vertical";
        }

        [[nodiscard]] std::string describeSize(const int width, const int height) {
            return std::to_string(width) + " by " + std::to_string(height);
        }
    } // namespace

    CoreException::CoreException(const std::string& message)
        : WarshipsException(ErrorLayer::Core, message) {}

    CoordinateOutOfBoardException::CoordinateOutOfBoardException(
            const Coordinate coordinate,
            const int boardWidth,
            const int boardHeight
    )
        : CoreException(
                  "cell " + describe(coordinate) + " is off a board of " +
                  describeSize(boardWidth, boardHeight)
          )
        , coordinate_(coordinate)
        , boardWidth_(boardWidth)
        , boardHeight_(boardHeight) {}

    Coordinate CoordinateOutOfBoardException::coordinate() const noexcept {
        return coordinate_;
    }

    int CoordinateOutOfBoardException::boardWidth() const noexcept {
        return boardWidth_;
    }

    int CoordinateOutOfBoardException::boardHeight() const noexcept {
        return boardHeight_;
    }

    ShipOverlapException::ShipOverlapException(
            const Coordinate origin,
            const Direction direction,
            const int length
    )
        : CoreException(
                  "a ship of " + std::to_string(length) + " laid " + describe(direction) +
                  " from " + describe(origin) + " meets one already there"
          )
        , origin_(origin)
        , direction_(direction)
        , length_(length) {}

    Coordinate ShipOverlapException::origin() const noexcept {
        return origin_;
    }

    Direction ShipOverlapException::direction() const noexcept {
        return direction_;
    }

    int ShipOverlapException::length() const noexcept {
        return length_;
    }

    ShipOutOfBoundsException::ShipOutOfBoundsException(
            const Coordinate origin,
            const Direction direction,
            const int length,
            const int boardWidth,
            const int boardHeight
    )
        : CoreException(
                  "a ship of " + std::to_string(length) + " laid " + describe(direction) +
                  " from " + describe(origin) + " runs off a board of " +
                  describeSize(boardWidth, boardHeight)
          )
        , origin_(origin)
        , direction_(direction)
        , length_(length)
        , boardWidth_(boardWidth)
        , boardHeight_(boardHeight) {}

    Coordinate ShipOutOfBoundsException::origin() const noexcept {
        return origin_;
    }

    Direction ShipOutOfBoundsException::direction() const noexcept {
        return direction_;
    }

    int ShipOutOfBoundsException::length() const noexcept {
        return length_;
    }

    int ShipOutOfBoundsException::boardWidth() const noexcept {
        return boardWidth_;
    }

    int ShipOutOfBoundsException::boardHeight() const noexcept {
        return boardHeight_;
    }

    CellAlreadyAttackedException::CellAlreadyAttackedException(const Coordinate coordinate)
        : CoreException("cell " + describe(coordinate) + " has already been fired on")
        , coordinate_(coordinate) {}

    Coordinate CellAlreadyAttackedException::coordinate() const noexcept {
        return coordinate_;
    }
} // namespace cpp_warships::core
