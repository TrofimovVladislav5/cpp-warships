#pragma once

#include <string>

namespace cpp_warships::model {
    /** @brief How an intent went. */
    class IntentResult {
       public:
        [[nodiscard]] static IntentResult succeeded();

        /** @brief A step that did not happen, and @p reason in words a player
         * could read. */
        [[nodiscard]] static IntentResult failed(std::string reason);

        [[nodiscard]] bool isSucceeded() const noexcept;
        [[nodiscard]] const std::string& reason() const noexcept;

       private:
        IntentResult(bool isSucceeded, std::string reason);

        bool isSucceeded_;
        std::string reason_;
    };
}  // namespace cpp_warships::model
