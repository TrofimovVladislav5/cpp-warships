#pragma once

#include <string>

namespace cpp_warships::serialization::helpers::serializers {
    /** @brief JsonStringSerializer is a utility class that provides methods to
     * extract field values from a serialized string representation. */
    class JsonStringSerializer {
    public:
        /** @brief Extracts the value of a specified field from a serialized string. */
        static std::string* extractFieldValue(
            std::string& data,
            const std::string& fieldName,
            bool isImplicit = false
        ) {
            size_t fieldLength = fieldName.length();
            size_t separatorLength = 2;
            size_t startPos = data.find(fieldName);
            size_t finalStartPos = startPos + fieldLength + separatorLength;

            if (startPos == std::string::npos) {
                return nullptr;
            } else {
                size_t endPos = data.find(isImplicit ? "};" : ";", finalStartPos);
                if (endPos == std::string::npos) {
                    endPos = data.length();
                }

                return new std::string(data.substr(finalStartPos, endPos - finalStartPos));
            }
        }

        /** @brief Sets the value for a specified field in a serialized string.
         * @return True if the field was found and set, false otherwise. */
        template <typename T = std::string>
        static bool setFieldValue(
            T* field,
            std::string& data,
            const std::string& fieldName,
            std::function<T(std::string&)> converter = [](std::string& value) { return value; }
        ) {
            if (auto value = extractFieldValue(data, fieldName)) {
                *field = converter(*value);
                delete value;
                return true;
            }

            return false;
        }

        /** @brief Checks if the serialized string contains all specified fields.          * @return
         * True if all specified fields are present, false otherwise. */
        template <typename... Fields>
        static bool isIncludeFields(const std::string& item, Fields... fields) {
            for (const std::string& field : {fields...}) {
                std::string expectedPrefix = field + ": ";

                if (item.find(expectedPrefix) == std::string::npos) {
                    return false;
                }
            }

            return true;
        }

        /** @brief Checks if the string is an item serialized in a class format.
         * @return True if an argument is a related item, false otherwise. */
        static bool isRelatedItem(const std::string& item) {
            return item.find('{') != std::string::npos && item.find('}') != std::string::npos &&
                   item.find(':') != std::string::npos;
        }

        /** @brief Serializes a map of fields into a string representation.          *  @return A
         * string representation of the serialized fields in a JSON-like format. */
        static std::string serializeFields(
            const std::unordered_map<std::string, std::string>& fields
        ) {
            std::string result;
            for (const auto& [title, value] : fields) {
                std::string fieldRepresentation = std::string(title).append(": ").append(value);
                result += fieldRepresentation;

                if (!isRelatedItem(fieldRepresentation)) {
                    result += ";\n";
                }
            }

            return "{\n" + result + "};\n";
        }
    };
}  // namespace cpp_warships::serialization::helpers::serializers