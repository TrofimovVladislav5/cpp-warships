#include <application/head/PresentationContext.h>

#include <application/flow/MatchPhase.h>

namespace cpp_warships::head {
    PresentationContext::PresentationContext(
            const model::ApplicationContext& application
    ) noexcept
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

    PresentationState& PresentationContext::state() noexcept {
        return state_;
    }

    const PresentationState& PresentationContext::state() const noexcept {
        return state_;
    }

    GridGeometry& PresentationContext::geometry() noexcept {
        return geometry_;
    }

    const GridGeometry& PresentationContext::geometry() const noexcept {
        return geometry_;
    }

    ScreenKind PresentationContext::currentScreen() const {
        if (state_.isAtMenu || !game().hasMatch()) {
            return ScreenKind::Menu;
        }

        return game().match().phase() == flow::MatchPhase::Placement ? ScreenKind::Placement
                                                                     : ScreenKind::Battle;
    }
} // namespace cpp_warships::head
