#include "../include/MatchSettingsController.h"

namespace cpp_warships::application {

    MatchSettingsController::MatchSettingsController() {}

    MatchSettingsController::~MatchSettingsController() {}

    std::map<int, int> MatchSettingsController::calculateOptimalLengthShips(int fieldSize) {
        std::map<int, int> shipsCounts;

        const float SHIP_COVERED_AREA_RATIO = 0.2f;
        float totalArea = static_cast<float>(fieldSize) * static_cast<float>(fieldSize);
        int totalShipArea = static_cast<int>(totalArea * SHIP_COVERED_AREA_RATIO);
        std::vector shipLengths = {4, 3, 2, 1};
        std::vector baseShipCounts = {1, 2, 3, 4};
        float scalingFactor = static_cast<float>(fieldSize) / 10.0f;

        for (size_t i = 0; i < shipLengths.size(); ++i) {
            int scaledCount = static_cast<int>(baseShipCounts[i] * scalingFactor);
            int maxPossibleShips = std::min(scaledCount, totalShipArea / shipLengths[i]);

            shipsCounts[shipLengths[i]] = maxPossibleShips;
            totalShipArea -= maxPossibleShips * shipLengths[i];
        }

        return shipsCounts;
    }

    MatchSettings* MatchSettingsController::createMatchSettings(int fieldSize) {
        std::map<int, int> shipsCount = calculateOptimalLengthShips(fieldSize);

        auto* settings = new MatchSettings(shipsCount, fieldSize);
        return settings;
    }
} // namespace cpp_warships::application