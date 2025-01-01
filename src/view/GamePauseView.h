#pragma once
#include <string>

class GamePauseView {
private:
    int rawLength;

public:
    explicit GamePauseView(int rawLength);
    void printImportantMessage(const std::string &message) const;
};
