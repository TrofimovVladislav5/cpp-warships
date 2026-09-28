#include <application/model/intents/IntentResult.h>

#include <utility>

namespace cpp_warships::model {
    IntentResult::IntentResult(const bool isSucceeded, std::string reason)
        : isSucceeded_(isSucceeded), reason_(std::move(reason)) {
    }

    IntentResult IntentResult::succeeded() {
        return IntentResult{true, {}};
    }

    IntentResult IntentResult::failed(std::string reason) {
        return IntentResult{false, std::move(reason)};
    }

    bool IntentResult::isSucceeded() const noexcept {
        return isSucceeded_;
    }

    const std::string& IntentResult::reason() const noexcept {
        return reason_;
    }
}  // namespace cpp_warships::model
