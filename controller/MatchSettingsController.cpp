#include "MatchSettingsController.h"
#include "../library/parser/Parser.h"


MatchSettingsController::MatchSettingsController() {}

MatchSettingsController::~MatchSettingsController() {}

std::vector<int> MatchSettingsController::calculateOptimalLengthShips(int fieldSize) {
    std::vector<int> shipsCounts;

    const float SHIP_COVERED_AREA_RATIO = 0.2f;
    float totalArea = static_cast<float>(fieldSize) * static_cast<float>(fieldSize);
    int totalShipArea = static_cast<int>(totalArea * SHIP_COVERED_AREA_RATIO);
    std::vector shipLengths = {4, 3, 2, 1};
    std::vector baseShipCounts = {1, 2, 3, 4};
    float scalingFactor = static_cast<float>(fieldSize) / 10.0f;

    for (size_t i = 0; i < shipLengths.size(); ++i) {
        int scaledCount = static_cast<int>(baseShipCounts[i] * scalingFactor);
        int maxPossibleShips = std::min(scaledCount, totalShipArea / shipLengths[i]);

        shipsCounts.push_back(maxPossibleShips);
        totalShipArea -= maxPossibleShips * shipLengths[i];
    }

    std::sort(shipsCounts.begin(), shipsCounts.end(), std::greater<int>());
    return shipsCounts;
}

MatchSettings* MatchSettingsController::createMatchSettings(int fieldSize) {
    std::vector<int> shipsCount = calculateOptimalLengthShips(fieldSize);

    std::vector<int> temp;
    for (int i = 0; i < shipsCount.size(); i++) {
        for (int j = 0; j < shipsCount[i]; j++) {
            temp.push_back(i + 1);
        }
    }

    auto* settings = new MatchSettings(fieldSize);
    settings->setPlayerManager(temp);
    settings->setFieldSize(fieldSize);
    return settings;
}
