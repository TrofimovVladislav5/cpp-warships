#include <application/flow/SkillBehaviour.h>

#include <unordered_map>
#include <vector>

namespace cpp_warships::flow {
    namespace {
        /** @brief How far from the scanned cell the scanner reaches. */
        constexpr int SCANNER_RADIUS = 1;

        /** @brief Picks a cell on @p board that has not been attacked yet. */
        std::optional<core::Coordinate> pickRandomUnattackedCell(
                const core::Board& board,
                RandomEngine& randomEngine
        ) {
            std::vector<core::Coordinate> available;
            for (int row = 0; row < board.height(); ++row) {
                for (int column = 0; column < board.width(); ++column) {
                    const core::Coordinate coordinate{column, row};
                    if (!board.attackedCells().contains(coordinate)) {
                        available.push_back(coordinate);
                    }
                }
            }

            std::optional<core::Coordinate> chosen;
            if (!available.empty()) {
                std::uniform_int_distribution<std::size_t> distribution{0, available.size() - 1};
                chosen = available[distribution(randomEngine)];
            }

            return chosen;
        }

        /** @brief Reports whether the scanned area holds a ship, without firing. */
        class ScannerBehaviour final : public SkillBehaviour {
        public:
            [[nodiscard]] bool needsTarget() const override {
                return true;
            }

            void apply(
                    SkillContext& context,
                    std::optional<core::Coordinate> target
            ) const override {
                const bool foundShip = context.enemyBoard().hasShipWithin(*target, SCANNER_RADIUS);
                context.recordSkillEvent(
                        {.kind = MatchEventKind::AreaScanned,
                         .actor = Participant::Player,
                         .coordinate = target,
                         .skill = SkillKind::Scanner,
                         .scanFoundShip = foundShip}
                );
            }
        };

        /** @brief Arms the next shot to deal twice its usual damage. */
        class DoubleDamageBehaviour final : public SkillBehaviour {
        public:
            [[nodiscard]] bool needsTarget() const override {
                return false;
            }

            void apply(SkillContext& context, std::optional<core::Coordinate>) const override {
                context.armDoubleDamage();
                context.recordSkillEvent(
                        {.kind = MatchEventKind::DoubleDamageArmed,
                         .actor = Participant::Player,
                         .skill = SkillKind::DoubleDamage}
                );
            }
        };

        /** @brief Fires at a random enemy cell, which may well miss.
         *  The shot reports itself, so this adds no event of its own. */
        class RandomStrikeBehaviour final : public SkillBehaviour {
        public:
            [[nodiscard]] bool needsTarget() const override {
                return false;
            }

            void apply(SkillContext& context, std::optional<core::Coordinate>) const override {
                const std::optional<core::Coordinate> targetCell =
                        pickRandomUnattackedCell(context.enemyBoard(), context.randomEngine());
                if (targetCell.has_value()) {
                    context.strikeEnemyCell(*targetCell);
                }
            }
        };

        const ScannerBehaviour SCANNER_BEHAVIOUR;
        const DoubleDamageBehaviour DOUBLE_DAMAGE_BEHAVIOUR;
        const RandomStrikeBehaviour RANDOM_STRIKE_BEHAVIOUR;

        const std::unordered_map<SkillKind, const SkillBehaviour*> BEHAVIOUR_BY_SKILL{
                {SkillKind::Scanner, &SCANNER_BEHAVIOUR},
                {SkillKind::DoubleDamage, &DOUBLE_DAMAGE_BEHAVIOUR},
                {SkillKind::RandomStrike, &RANDOM_STRIKE_BEHAVIOUR}
        };
    } // namespace

    const SkillBehaviour& behaviourFor(SkillKind skill) {
        return *BEHAVIOUR_BY_SKILL.at(skill);
    }
} // namespace cpp_warships::flow
