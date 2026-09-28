#include <application/model/ApplicationContext.h>

#include <cstddef>
#include <utility>

namespace cpp_warships::model {
    namespace {
        /** @brief How many notices are worth keeping. Older ones have been on
         * screen long enough to have been read, or long enough not to matter.
         */
        constexpr std::size_t MOST_NOTICES_KEPT = 8;
    }  // namespace

    ApplicationContext::ApplicationContext(WarshipsGame& game) noexcept : game_(game) {
    }

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

    void ApplicationContext::note(std::string message) {
        notices_.push_back(std::move(message));

        while (notices_.size() > MOST_NOTICES_KEPT) {
            notices_.pop_front();
        }
    }

    const std::deque<std::string>& ApplicationContext::notices() const noexcept {
        return notices_;
    }

    void ApplicationContext::clearNotices() noexcept {
        notices_.clear();
    }
}  // namespace cpp_warships::model
