#include <application/model/MatchInPlay.h>

#include <utility>

#include <application/model/errors/ModelExceptions.h>

namespace cpp_warships::model {
    MatchInPlay::MatchInPlay(flow::RandomEngine& randomEngine) noexcept
        : randomEngine_(randomEngine) {}

    bool MatchInPlay::hasMatch() const noexcept {
        return match_.has_value();
    }

    const flow::Match& MatchInPlay::match() const {
        if (!match_.has_value()) {
            throw NoMatchInPlayException();
        }

        return *match_;
    }

    flow::Match& MatchInPlay::editableMatch() {
        if (!match_.has_value()) {
            throw NoMatchInPlayException();
        }

        return *match_;
    }

    const BattleJournal& MatchInPlay::journal() const noexcept {
        return journal_;
    }

    flow::RandomEngine& MatchInPlay::randomEngine() const noexcept {
        return randomEngine_;
    }

    void MatchInPlay::replaceWith(flow::Match match) {
        match_.emplace(std::move(match));
        journal_.clear();
    }

    void MatchInPlay::recordEvents() {
        journal_.absorb(editableMatch().drainEvents());
    }
} // namespace cpp_warships::model
