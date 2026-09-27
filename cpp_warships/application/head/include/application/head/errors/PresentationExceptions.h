#pragma once

#include <string>

#include <application/core/errors/WarshipsException.h>

namespace cpp_warships::head {
    /** @brief Something went wrong in showing the game rather than in playing it.
     *  Nothing below this layer throws one, and none of these reach the model. */
    class PresentationException : public core::WarshipsException {
    protected:
        explicit PresentationException(const std::string& message);
    };

    /** @brief The host the interface runs in could not do what was asked of it. */
    class ShellException final : public PresentationException {
    public:
        explicit ShellException(const std::string& cause);

        [[nodiscard]] const std::string& cause() const noexcept;

    private:
        std::string cause_;
    };

    /** @brief Input arrived that could be read but not made sense of, such as a click on a
     *  board whose geometry is not known yet. An unbound key is not this: that is simply ignored. */
    class EventMappingException final : public PresentationException {
    public:
        explicit EventMappingException(const std::string& cause);

        [[nodiscard]] const std::string& cause() const noexcept;

    private:
        std::string cause_;
    };
} // namespace cpp_warships::head
