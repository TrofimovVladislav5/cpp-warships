#include <application/flow/SkillManager.h>

#include <utility>

namespace cpp_warships::flow {
    SkillManager::SkillManager(RandomEngine& randomEngine)
        : randomEngine_(randomEngine) {}

    SkillManager::SkillManager(RandomEngine& randomEngine, SkillQueue bank)
        : randomEngine_(randomEngine)
        , bank_(std::move(bank)) {}

    void SkillManager::grantOpeningHand() {
        bank_.grantAllShuffled(randomEngine_);
    }

    SkillKind SkillManager::grantRandom() {
        return bank_.grantRandom(randomEngine_);
    }

    const SkillQueue& SkillManager::bank() const noexcept {
        return bank_;
    }

    bool SkillManager::nextNeedsTarget() const {
        const std::optional<SkillKind> pending = bank_.next();
        return pending.has_value() && behaviourFor(*pending).needsTarget();
    }

    bool SkillManager::applyNext(SkillContext& context, std::optional<core::Coordinate> target) {
        const std::optional<SkillKind> pending = bank_.next();
        bool isApplied = false;

        if (pending.has_value()) {
            const SkillBehaviour& behaviour = behaviourFor(*pending);
            if (!behaviour.needsTarget() || target.has_value()) {
                bank_.consume();
                behaviour.apply(context, target);
                isApplied = true;
            }
        }

        return isApplied;
    }
}  // namespace cpp_warships::flow
