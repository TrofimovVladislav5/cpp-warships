#include "GamePauseView.h"

#include <cmath>

#include "ViewHelper.h"

GamePauseView::GamePauseView(int rawLength) : rawLength(rawLength) {}

void GamePauseView::printImportantMessage(const std::string &message) const {
    int lineLength = std::max(static_cast<int>(std::floor(rawLength - message.length())) / 2, 1);
    ViewHelper::consoleOut("\n" + std::string(lineLength, ' ') + ' ' + message + ' ' +
                           std::string(lineLength, ' ') + "\n");
}
