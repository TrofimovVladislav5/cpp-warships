#include <application/head/screens/ScreenNavigator.h>

#include <stdexcept>
#include <utility>

namespace cpp_warships::head {
    void ScreenNavigator::add(std::unique_ptr<Screen> screen) {
        const ScreenKind kind = screen->kind();
        screens_[kind] = std::move(screen);
    }

    void ScreenNavigator::showScreen(ScreenKind screen) {
        if (knows(screen)) {
            currentScreen_ = screen;
        }
    }

    ScreenKind ScreenNavigator::currentScreen() const noexcept {
        return currentScreen_;
    }

    bool ScreenNavigator::knows(ScreenKind screen) const {
        return screens_.contains(screen);
    }

    Screen& ScreenNavigator::activeScreen() {
        const auto found = screens_.find(currentScreen_);
        if (found == screens_.end()) {
            throw std::logic_error("ScreenNavigator::activeScreen: no screen is showing");
        }

        return *found->second;
    }
} // namespace cpp_warships::head
