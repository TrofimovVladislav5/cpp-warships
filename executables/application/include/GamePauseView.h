#pragma once

#include <string>

namespace cpp_warships::application {

    class GamePauseView {
    private:
        int rawLength;

    public:
        explicit GamePauseView(int rawLength);
        void printImportantMessage(const std::string &message) const;
    };
} // namespace cpp_warships::application