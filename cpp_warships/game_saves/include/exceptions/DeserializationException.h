#pragma once

#include <exception>
#include <string>

namespace cpp_warships::game_saves::exceptions {
    /**
     * @brief Exception thrown when deserialization of a specific object fails.
     */
    class DeserializationException : public std::exception {
    private:
        std::string message;
        std::string object_type;

    public:
        /**
         * @brief Constructs a DeserializationException with a given message.
         * @param type The type of the object that failed to deserialize.
         * @param message The error message.
         */
        explicit DeserializationException(const std::string& type, const std::string& message)
            : message(message)
            , object_type(type)
        {}

        /**
         * @brief Returns the error message.
         * @return The error message.
         */
        [[nodiscard]] const char* what() const noexcept override {
            return (object_type + " deserialization error: " + message).c_str();
        }
    };

} // namespace cpp_warships::game_saves::exceptions