#include <application/model/errors/ModelExceptions.h>

namespace cpp_warships::model::errors {
    ModelException::ModelException(const std::string& message)
        : core::errors::WarshipsException(core::errors::ErrorLayer::Model, message) {}

    NoMatchInPlayException::NoMatchInPlayException()
        : ModelException("there is no match in play") {}
}  // namespace cpp_warships::model::errors
