#pragma once

#include <application/model/WarshipsGame.h>

namespace cpp_warships::model {
    /** @brief Everything an intent is allowed to reach, and nothing else. What an intent
     *  cannot get to from here, it cannot change: that is the whole point of the type. */
    class ApplicationContext {
    public:
        /** @brief A context over @p game, which must outlive it. */
        explicit ApplicationContext(WarshipsGame& game) noexcept;

        [[nodiscard]] WarshipsGame& game() noexcept;
        [[nodiscard]] const WarshipsGame& game() const noexcept;

        /** @brief Whether the session has been asked to end. The host watches this rather
         *  than being told, which keeps quitting behind whatever was queued before it. */
        [[nodiscard]] bool isFinished() const noexcept;
        void finish() noexcept;

    private:
        WarshipsGame& game_;
        bool isFinished_ = false;
    };
} // namespace cpp_warships::model
