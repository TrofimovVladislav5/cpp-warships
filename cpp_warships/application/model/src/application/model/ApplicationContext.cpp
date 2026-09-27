#include <application/model/ApplicationContext.h>

namespace cpp_warships::model {
    ApplicationContext::ApplicationContext(WarshipsGame& game) noexcept
        : game_(game) {}

    WarshipsGame& ApplicationContext::game() noexcept {
        return game_;
    }

    const WarshipsGame& ApplicationContext::game() const noexcept {
        return game_;
    }

    bool ApplicationContext::isFinished() const noexcept {
        return isFinished_;
    }

    void ApplicationContext::finish() noexcept {
        isFinished_ = true;
    }
} // namespace cpp_warships::model
