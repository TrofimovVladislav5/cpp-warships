#include <application/persistence/errors/PersistenceExceptions.h>

namespace cpp_warships::persistence {
    PersistenceException::PersistenceException(
            const std::string& slotName,
            const std::string& message
    )
        : WarshipsException(core::ErrorLayer::Persistence, "save '" + slotName + "': " + message)
        , slotName_(slotName) {}

    const std::string& PersistenceException::slotName() const noexcept {
        return slotName_;
    }

    SaveNotFoundException::SaveNotFoundException(const std::string& slotName)
        : PersistenceException(slotName, "there is nothing saved under that name") {}

    SaveWriteException::SaveWriteException(
            const std::string& slotName,
            const std::string& cause
    )
        : PersistenceException(slotName, "could not be written: " + cause)
        , cause_(cause) {}

    const std::string& SaveWriteException::cause() const noexcept {
        return cause_;
    }

    SaveCorruptException::SaveCorruptException(
            const std::string& slotName,
            const std::string& cause
    )
        : PersistenceException(slotName, "does not describe a match any more: " + cause)
        , cause_(cause) {}

    const std::string& SaveCorruptException::cause() const noexcept {
        return cause_;
    }
} // namespace cpp_warships::persistence
