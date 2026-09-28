#pragma once

#include <application/model/WarshipsGame.h>

#include <deque>
#include <string>

namespace cpp_warships::model {
    /** @brief Everything an intent is allowed to reach, and nothing else. What
     * an intent cannot get to from here, it cannot change: that is the whole
     * point of the type. */
    class ApplicationContext {
       public:
        /** @brief A context over @p game, which must outlive it. */
        explicit ApplicationContext(WarshipsGame& game) noexcept;

        [[nodiscard]] WarshipsGame& game() noexcept;
        [[nodiscard]] const WarshipsGame& game() const noexcept;

        /** @brief Whether the session has been asked to end. The host watches
         * this rather than being told, which keeps quitting behind whatever was
         * queued before it. */
        [[nodiscard]] bool isFinished() const noexcept;
        void finish() noexcept;

        /** @brief Says that @p message is worth telling the player. This is how
         * a failure reaches the interface: as something to read, not as
         * something to catch. */
        void note(std::string message);

        /** @brief What is worth telling the player, oldest first. */
        [[nodiscard]] const std::deque<std::string>& notices() const noexcept;
        void clearNotices() noexcept;

       private:
        WarshipsGame& game_;
        bool isFinished_ = false;
        std::deque<std::string> notices_;
    };
}  // namespace cpp_warships::model
