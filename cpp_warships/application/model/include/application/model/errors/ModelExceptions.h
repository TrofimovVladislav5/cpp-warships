#pragma once

#include <application/core/errors/WarshipsException.h>

#include <string>

namespace cpp_warships::model::errors {
    /** @brief Something the model was asked to do that it had no way to do. */
    class ModelException : public core::errors::WarshipsException {
    protected:
        explicit ModelException(const std::string& message);
    };

    /** @brief The match in play was reached for when there is none. */
    class NoMatchInPlayException final : public ModelException {
    public:
        NoMatchInPlayException();
    };
}  // namespace cpp_warships::model::errors
