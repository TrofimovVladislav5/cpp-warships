#include <application/core/errors/ErrorLayer.h>

namespace cpp_warships::core {
    std::string_view nameOf(const ErrorLayer layer) noexcept {
        switch (layer) {
            case ErrorLayer::Core:
                return "core";
            case ErrorLayer::Flow:
                return "flow";
            case ErrorLayer::Persistence:
                return "persistence";
            case ErrorLayer::Model:
                return "model";
            case ErrorLayer::Presentation:
                return "presentation";
        }

        return "unknown";
    }
} // namespace cpp_warships::core
