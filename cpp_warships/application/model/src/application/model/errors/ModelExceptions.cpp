#include <application/model/errors/ModelExceptions.h>

namespace cpp_warships::model {
    ModelException::ModelException(const std::string& message)
        : WarshipsException(core::ErrorLayer::Model, message) {
    }

    NoMatchInPlayException::NoMatchInPlayException() : ModelException("there is no match in play") {
    }
}  // namespace cpp_warships::model
