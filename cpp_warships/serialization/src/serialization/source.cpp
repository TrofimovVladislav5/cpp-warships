#include <serialization/ISerializable.h>
#include <serialization/ISerializer.h>
#include <serialization/SerializerAggregator.h>
#include <serialization/example/ImplicitTestClass.h>
#include <serialization/example/TestClass.h>
#include <serialization/exceptions/DeserializationException.h>
#include <serialization/exceptions/InterpretationException.h>
#include <serialization/exceptions/SerializationException.h>
#include <serialization/helpers/TupleBuilder.h>
#include <serialization/helpers/serializers/JsonStringSerializer.h>
#include <serialization/helpers/type_converters/StringTypeConverter.h>

namespace cpp_warships::serialization {
    /** @brief The one symbol this library defines. */
    const char* libraryName() noexcept {
        return "serialization";
    }
}  // namespace cpp_warships::serialization
