#pragma once

#include <string>

#include <application/core/errors/WarshipsException.h>

namespace cpp_warships::persistence {
    /** @brief A save could not be read or written. Errors thrown by the serialization
     *  library are wrapped into these, so that nothing generic escapes this layer. */
    class PersistenceException : public core::WarshipsException {
    public:
        /** @brief Which save was being read or written. */
        [[nodiscard]] const std::string& slotName() const noexcept;

    protected:
        PersistenceException(const std::string& slotName, const std::string& message);

    private:
        std::string slotName_;
    };

    /** @brief There is no save under that name. */
    class SaveNotFoundException final : public PersistenceException {
    public:
        explicit SaveNotFoundException(const std::string& slotName);
    };

    /** @brief A save exists but could not be written to. */
    class SaveWriteException final : public PersistenceException {
    public:
        SaveWriteException(const std::string& slotName, const std::string& cause);

        /** @brief What the underlying library said, kept as text rather than as a nested
         *  exception: enough to diagnose, and nothing for a caller to have to catch. */
        [[nodiscard]] const std::string& cause() const noexcept;

    private:
        std::string cause_;
    };

    /** @brief A save was found but does not describe a match any more. */
    class SaveCorruptException final : public PersistenceException {
    public:
        SaveCorruptException(const std::string& slotName, const std::string& cause);

        [[nodiscard]] const std::string& cause() const noexcept;

    private:
        std::string cause_;
    };
} // namespace cpp_warships::persistence
