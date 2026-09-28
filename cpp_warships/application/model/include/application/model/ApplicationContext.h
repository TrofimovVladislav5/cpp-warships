#pragma once

#include <deque>
#include <string>

namespace cpp_warships::model {
    class WarshipsGame;
}

namespace cpp_warships::model {
    /** @brief Everything an intent is allowed to reach, and nothing else. */
    class ApplicationContext {
    public:
        /** @brief A context over @p game, which must outlive it. */
        explicit ApplicationContext(WarshipsGame& game) noexcept;

        [[nodiscard]] WarshipsGame& game() noexcept;
        [[nodiscard]] const WarshipsGame& game() const noexcept;

        /** @brief Whether the session has been asked to end. */
        [[nodiscard]] bool isFinished() const noexcept;
        void finish() noexcept;

        /** @brief Says that @p message is worth telling the player. */
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
