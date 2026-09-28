#pragma once

#include <serialization/exceptions/InterpretationException.h>

#include <string>

namespace cpp_warships::serialization {
    class ISerializableBase;

    template <char* TName = nullptr>
    class ISerializableTyped;

    template <typename T>
    struct is_serializable_derivative;

    template <char* TName = nullptr>
    class ISerializable;

    /** @brief Pure virtual base class that doesn't use any template parameters.
     */
    class ISerializableBase {
       public:
        virtual ~ISerializableBase() = default;
        [[nodiscard]] virtual std::string getType() = 0;
    };

    /** @brief Base class for typed serializers implementing the getType method.
     */
    template <char* TName>
    class ISerializableTyped : public ISerializableBase {
       public:
        [[nodiscard]] std::string getType() override {
            if constexpr (TName != nullptr) {
                return TName;
            } else {
                throw exceptions::InterpretationException(
                    "Trying to get type from ISerializableTyped with nullptr "
                    "TName."
                );
            }
        }
    };

    /** @brief Trait to check if a type is a derivative of ISerializableBase. */
    template <typename T>
    struct is_serializable_derivative {
       private:
        static std::true_type test(ISerializableBase*);
        static std::false_type test(...);

       public:
        static constexpr bool value = decltype(test(std::declval<T*>()))::value;
    };

    /** @brief Concept to check if a type is a serializable derivative. */
    template <typename T>
    concept SerializableDerivative = is_serializable_derivative<T>::value;

    /** @brief Interface for serializable types, providing generic methods for
     * serialization and deserialization. */
    template <char* TName>
    class ISerializable : public ISerializableTyped<TName> {};
}  // namespace cpp_warships::serialization
