#include <application/model/MatchInPlay.h>
#include <application/model/errors/ModelExceptions.h>

#include <utility>

namespace cpp_warships::model {
    MatchInPlay::MatchInPlay(flow::RandomEngine& randomEngine) noexcept
        : randomEngine_(randomEngine) {}

    bool MatchInPlay::hasMatch() const noexcept {
        return match_.has_value();
    }

    const flow::Match& MatchInPlay::match() const {
        if (!match_.has_value()) {
            throw errors::NoMatchInPlayException();
        }

        return *match_;
    }

    flow::Match& MatchInPlay::editableMatch() {
        if (!match_.has_value()) {
            throw errors::NoMatchInPlayException();
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
        loadedFrom_.reset();
    }

    void MatchInPlay::replaceWith(
        flow::Match match,
        const flow::MatchEventLog& story,
        std::string fromSlot
    ) {
        replaceWith(std::move(match));
        journal_.absorb(story);
        loadedFrom_ = std::move(fromSlot);
    }

    const std::optional<std::string>& MatchInPlay::loadedFrom() const noexcept {
        return loadedFrom_;
    }

    void MatchInPlay::rememberSlot(std::string slot) {
        loadedFrom_ = std::move(slot);
    }

    void MatchInPlay::recordEvents() {
        journal_.absorb(editableMatch().drainEvents());
    }
}  // namespace cpp_warships::model
