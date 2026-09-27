#pragma once

#include <string>

#include <application/core/Coordinate.h>
#include <application/core/Direction.h>
#include <application/core/errors/WarshipsException.h>

namespace cpp_warships::core {
    /** @brief A rule of the board broken. Every error raised by the rules is one of these,
     *  which is what lets the layer above catch the lot without naming each one. */
    class CoreException : public WarshipsException {
    protected:
        explicit CoreException(const std::string& message);
    };

    /** @brief A cell was named that the board does not have. */
    class CoordinateOutOfBoardException final : public CoreException {
    public:
        CoordinateOutOfBoardException(
                Coordinate coordinate,
                int boardWidth,
                int boardHeight
        );

        [[nodiscard]] Coordinate coordinate() const noexcept;
        [[nodiscard]] int boardWidth() const noexcept;
        [[nodiscard]] int boardHeight() const noexcept;

    private:
        Coordinate coordinate_;
        int boardWidth_;
        int boardHeight_;
    };

    /** @brief A ship was laid where another already lies. */
    class ShipOverlapException final : public CoreException {
    public:
        ShipOverlapException(Coordinate origin, Direction direction, int length);

        [[nodiscard]] Coordinate origin() const noexcept;
        [[nodiscard]] Direction direction() const noexcept;
        [[nodiscard]] int length() const noexcept;

    private:
        Coordinate origin_;
        Direction direction_;
        int length_;
    };

    /** @brief A ship was laid so that part of it would hang off the board. */
    class ShipOutOfBoundsException final : public CoreException {
    public:
        ShipOutOfBoundsException(
                Coordinate origin,
                Direction direction,
                int length,
                int boardWidth,
                int boardHeight
        );

        [[nodiscard]] Coordinate origin() const noexcept;
        [[nodiscard]] Direction direction() const noexcept;
        [[nodiscard]] int length() const noexcept;
        [[nodiscard]] int boardWidth() const noexcept;
        [[nodiscard]] int boardHeight() const noexcept;

    private:
        Coordinate origin_;
        Direction direction_;
        int length_;
        int boardWidth_;
        int boardHeight_;
    };

    /** @brief A cell was fired on that had already been fired on. */
    class CellAlreadyAttackedException final : public CoreException {
    public:
        explicit CellAlreadyAttackedException(Coordinate coordinate);

        [[nodiscard]] Coordinate coordinate() const noexcept;

    private:
        Coordinate coordinate_;
    };
} // namespace cpp_warships::core
