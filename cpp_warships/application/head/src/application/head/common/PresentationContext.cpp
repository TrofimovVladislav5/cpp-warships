#include <application/flow/MatchPhase.h>
#include <application/head/common/PresentationContext.h>
#include <application/model/ApplicationContext.h>
#include <application/model/WarshipsGame.h>

namespace cpp_warships::head::common {
    PresentationContext::PresentationContext(const model::ApplicationContext& application) noexcept
        : application_(application) {}

    const model::ApplicationContext& PresentationContext::application() const noexcept {
        return application_;
    }

    const model::WarshipsGame& PresentationContext::game() const noexcept {
        return application_.game();
    }

    const Theme& PresentationContext::theme() const noexcept {
        return theme_.current();
    }

    ThemeSelection& PresentationContext::themeSelection() noexcept {
        return theme_;
    }

    state::PresentationState& PresentationContext::state() noexcept {
        return state_;
    }

    const state::PresentationState& PresentationContext::state() const noexcept {
        return state_;
    }

    input::GridGeometry& PresentationContext::geometry() noexcept {
        return geometry_;
    }

    const input::GridGeometry& PresentationContext::geometry() const noexcept {
        return geometry_;
    }

    ScreenKind PresentationContext::currentScreen() const {
        if (state_.isAtMenu || !game().hasMatch()) {
            return ScreenKind::Menu;
        }

        return game().match().phase() == flow::MatchPhase::Placement ? ScreenKind::Placement
                                                                     : ScreenKind::Battle;
    }
}  // namespace cpp_warships::head::common
