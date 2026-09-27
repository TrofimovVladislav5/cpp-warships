#include <game_flow/AiOpponent.h>

#include <game_flow/AttackOutcomeBehaviour.h>

#include <algorithm>
#include <array>
#include <limits>

namespace cpp_warships::game_flow {

    AiOpponent::AiOpponent(RandomEngine& randomEngine)
        : randomEngine_(randomEngine) {}

    std::optional<game_core::Coordinate> AiOpponent::unfinishedHit(
            const game_core::Board& board
    ) const {
        const auto isStillHolding = [&board](const game_core::Coordinate& hit) {
            const auto cellState = board.stateAt(hit, game_core::Visibility::Opponent);
            return cellState == game_core::CellState::Damaged;
        };

        const auto damagedCell =
                std::find_if(currentTargetHits_.begin(), currentTargetHits_.end(), isStillHolding);

        if (damagedCell == currentTargetHits_.end()) {
            return std::nullopt;
        } else {
            return std::optional{*damagedCell};
        }
    }

    std::vector<game_core::Coordinate> AiOpponent::untriedNeighbours(
            game_core::Coordinate coordinate,
            const game_core::Board& board
    ) const {
        const std::array<game_core::Coordinate, 4> candidates{
                game_core::Coordinate{coordinate.x - 1, coordinate.y},
                game_core::Coordinate{coordinate.x + 1, coordinate.y},
                game_core::Coordinate{coordinate.x, coordinate.y - 1},
                game_core::Coordinate{coordinate.x, coordinate.y + 1}
        };

        std::vector<game_core::Coordinate> neighbours;
        for (const game_core::Coordinate& candidate : candidates) {
            if (board.contains(candidate) && !attemptedCoordinates_.contains(candidate)) {
                neighbours.push_back(candidate);
            }
        }

        return neighbours;
    }

    std::optional<game_core::Coordinate> AiOpponent::continueAlongHits(
            const game_core::Board& board
    ) const {
        const bool isVerticalRun = currentTargetHits_[0].x == currentTargetHits_[1].x;

        int lowestAlongRun = std::numeric_limits<int>::max();
        int highestAlongRun = std::numeric_limits<int>::min();
        for (const game_core::Coordinate& hit : currentTargetHits_) {
            const int positionAlongRun = isVerticalRun ? hit.y : hit.x;
            lowestAlongRun = std::min(lowestAlongRun, positionAlongRun);
            highestAlongRun = std::max(highestAlongRun, positionAlongRun);
        }

        const game_core::Coordinate anyHit = currentTargetHits_.front();
        const int fixedAxis = isVerticalRun ? anyHit.x : anyHit.y;
        const std::array<game_core::Coordinate, 2> extensions{
                isVerticalRun ? game_core::Coordinate{fixedAxis, lowestAlongRun - 1}
                              : game_core::Coordinate{lowestAlongRun - 1, fixedAxis},
                isVerticalRun ? game_core::Coordinate{fixedAxis, highestAlongRun + 1}
                              : game_core::Coordinate{highestAlongRun + 1, fixedAxis}
        };

        for (const game_core::Coordinate& extension : extensions) {
            if (board.contains(extension) && !attemptedCoordinates_.contains(extension)) {
                return extension;
            }
        }

        return std::nullopt;
    }

    std::optional<game_core::Coordinate> AiOpponent::pickRandomUntried(
            const game_core::Board& board
    ) {
        std::vector<game_core::Coordinate> available;
        for (int row = 0; row < board.height(); ++row) {
            for (int column = 0; column < board.width(); ++column) {
                const game_core::Coordinate coordinate{column, row};
                if (!attemptedCoordinates_.contains(coordinate)) {
                    available.push_back(coordinate);
                }
            }
        }

        if (available.empty()) {
            return std::nullopt;
        }

        std::uniform_int_distribution<std::size_t> distribution{0, available.size() - 1};
        return available[distribution(randomEngine_)];
    }

    std::optional<game_core::Coordinate> AiOpponent::chooseTarget(const game_core::Board& board) {
        if (const std::optional<game_core::Coordinate> holding = unfinishedHit(board)) {
            return holding;
        }

        if (currentTargetHits_.size() == 1) {
            const std::vector<game_core::Coordinate> neighbours =
                    untriedNeighbours(currentTargetHits_.front(), board);

            if (!neighbours.empty()) {
                std::uniform_int_distribution<std::size_t> distribution{0, neighbours.size() - 1};

                return neighbours[distribution(randomEngine_)];
            }
        } else if (currentTargetHits_.size() > 1) {
            if (const std::optional<game_core::Coordinate> extension = continueAlongHits(board)) {
                return extension;
            }
        }

        return pickRandomUntried(board);
    }

    void AiOpponent::finishHunt(const game_core::Board& board) {
        for (const game_core::Coordinate& hit : currentTargetHits_) {
            for (int rowOffset = -1; rowOffset <= 1; ++rowOffset) {
                for (int columnOffset = -1; columnOffset <= 1; ++columnOffset) {
                    const game_core::Coordinate neighbour{hit.x + columnOffset, hit.y + rowOffset};

                    if (board.contains(neighbour)) {
                        attemptedCoordinates_.insert(neighbour);
                    }
                }
            }
        }

        currentTargetHits_.clear();
    }

    void AiOpponent::markAttempted(game_core::Coordinate coordinate) {
        attemptedCoordinates_.insert(coordinate);
    }

    void AiOpponent::registerHit(game_core::Coordinate coordinate) {
        const auto known =
                std::find(currentTargetHits_.begin(), currentTargetHits_.end(), coordinate);

        if (known == currentTargetHits_.end()) {
            currentTargetHits_.push_back(coordinate);
        }
    }

    void AiOpponent::recordOutcome(
            const game_core::Coordinate coordinate,
            const game_core::AttackOutcome outcome,
            const game_core::Board& board
    ) {
        const auto cellState = board.stateAt(coordinate, game_core::Visibility::Opponent);
        const bool isStillHolding = cellState == game_core::CellState::Damaged;
        if (!isStillHolding) {
            markAttempted(coordinate);
        }

        behaviourFor(outcome).updateHunt(*this, coordinate, board);
    }

} // namespace cpp_warships::game_flow
