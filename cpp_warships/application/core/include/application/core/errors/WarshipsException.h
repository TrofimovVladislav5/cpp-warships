#pragma once

#include <application/core/errors/ErrorLayer.h>

#include <exception>
#include <string>

namespace cpp_warships::core::errors {
    /** @brief What every error in the game is. Holding the layer it came from is what lets
     * each layer catch its own and let nothing from below pass through unanswered. */
    class WarshipsException : public std::exception {
    public:
        [[nodiscard]] ErrorLayer layer() const noexcept;
        [[nodiscard]] const char* what() const noexcept override;

    protected:
        /** @brief An error from @p layer, described by @p message.
         * Only a layer's own base exception builds one, so no error can forget its layer. */
        WarshipsException(ErrorLayer layer, const std::string& message);

    private:
        ErrorLayer layer_;
        std::string description_;
    };
}  // namespace cpp_warships::core::errors
