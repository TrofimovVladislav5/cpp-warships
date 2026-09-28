#include <application/core/errors/WarshipsException.h>

#include <string>

namespace cpp_warships::core {
    WarshipsException::WarshipsException(const ErrorLayer layer, const std::string& message)
        : layer_(layer)
        , description_(std::string{nameOf(layer)} + " error: " + message) {
    }

    ErrorLayer WarshipsException::layer() const noexcept {
        return layer_;
    }

    const char* WarshipsException::what() const noexcept {
        return description_.c_str();
    }
}  // namespace cpp_warships::core
