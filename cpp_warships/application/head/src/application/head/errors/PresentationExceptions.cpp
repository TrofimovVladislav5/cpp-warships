#include <application/head/errors/PresentationExceptions.h>

namespace cpp_warships::head {
    PresentationException::PresentationException(const std::string& message)
        : WarshipsException(core::ErrorLayer::Presentation, message) {}

    ShellException::ShellException(const std::string& cause)
        : PresentationException("the host could not carry on: " + cause)
        , cause_(cause) {}

    const std::string& ShellException::cause() const noexcept {
        return cause_;
    }

    EventMappingException::EventMappingException(const std::string& cause)
        : PresentationException("input could not be made sense of: " + cause)
        , cause_(cause) {}

    const std::string& EventMappingException::cause() const noexcept {
        return cause_;
    }
} // namespace cpp_warships::head
