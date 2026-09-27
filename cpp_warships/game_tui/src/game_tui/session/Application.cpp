#include <game_tui/session/Application.h>

#include <stdexcept>
#include <utility>

#include <game_core/MatchSettings.h>

namespace cpp_warships::game_tui {
    Application::Application(game_flow::RandomEngine& randomEngine)
        : randomEngine_(randomEngine)
        , theme_(defaultTheme()) {}

    const Theme& Application::theme() const noexcept {
        return theme_;
    }

    bool Application::hasMatch() const noexcept {
        return match_.has_value();
    }

    const game_flow::Match& Application::match() const {
        if (!match_.has_value()) {
            throw std::logic_error("Application::match called with no match in play");
        }

        return *match_;
    }

    const BattleJournal& Application::journal() const noexcept {
        return journal_;
    }

    void Application::startNewMatch(int boardSize) {
        match_.emplace(game_core::MatchSettings::forBoardSize(boardSize), randomEngine_);

        journal_.clear();
    }

    void Application::changeTheme(const std::string& themeName) {
        theme_ = themeNamed(themeName);
    }

    void Application::placeShip(
            game_core::Coordinate origin,
            game_core::Direction direction,
            int length
    ) {
        if (match_.has_value()) {
            match_->editablePlayerBoard()
                    .place(origin, direction, length, match_->settings().segmentHealth());
        }
    }

    void Application::removeShipAt(game_core::Coordinate coordinate) {
        if (match_.has_value()) {
            match_->editablePlayerBoard().removeShipAt(coordinate);
        }
    }

    void Application::shuffleFleet() {
        if (match_.has_value()) {
            match_->shufflePlayerFleet();
        }
    }

    bool Application::beginBattle() {
        if (!match_.has_value() || !match_->beginBattle()) {
            return false;
        }

        journal_.absorb(match_->drainEvents());
        return true;
    }

    void Application::fireAt(game_core::Coordinate coordinate) {
        if (match_.has_value()) {
            match_->fireAt(coordinate);
            settleTurn();
        }
    }

    void Application::useSkill(std::optional<game_core::Coordinate> target) {
        if (match_.has_value()) {
            match_->applyNextSkill(target);
            settleTurn();
        }
    }

    void Application::settleTurn() {
        match_->concludeTurn();
        journal_.absorb(match_->drainEvents());
    }
} // namespace cpp_warships::game_tui
