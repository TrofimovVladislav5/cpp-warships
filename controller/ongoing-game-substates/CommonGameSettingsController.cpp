#include "../library/parser/Parser.h"
#include "CommonGameSettingsController.h"
#include "../library/StringHelper.h"
#include "../library/parser/Parser.h"
#include "CommonGameSettingsController.h"
#include "../library/StringHelper.h"
#include <cmath>

CommonGameSettingsController::CommonGameSettingsController(StateContext& context) 
    : context(context)
{
    settings = new MatchSettings();
    context.currentMatch = new GameStateDTO(settings);
}

CommonGameSettingsController::~CommonGameSettingsController() {
    delete settings;
}

void CommonGameSettingsController::calculateOptimalLengthShips(int fieldSize, std::vector<int>& shipsCounts) {
    shipsCounts.clear();

    const float SHIP_AREA_RATIO = 0.2f; // fieldSize = 10, area = 100, default ships area = 4 * 1 + 3 * 2 + 2 * 3 + 1 * 4 = 20, ratio 20 / 100
    int totalArea = fieldSize * fieldSize;
    int totalShipArea = static_cast<int>(totalArea * SHIP_AREA_RATIO);
    std::vector<int> shipLengths = {4, 3, 2, 1};
    std::vector<int> baseShipCounts = {1, 2, 3, 4};
    float scalingFactor = static_cast<float>(fieldSize) / 10.0f;

    for (size_t i = 0; i < shipLengths.size(); ++i) {
        int scaledCount = static_cast<int>(baseShipCounts[i] * scalingFactor);
        int maxPossibleShips = std::min(scaledCount, totalShipArea / shipLengths[i]);

        shipsCounts.push_back(maxPossibleShips);
        totalShipArea -= maxPossibleShips * shipLengths[i];
    }
    std::sort(shipsCounts.begin(), shipsCounts.end(), std::greater<int>()); // the reverse order is needed, since we started calculating from the maximum to the minimum length.
}

void CommonGameSettingsController::handleGameFieldSize(ParsedOptions options){
    std::vector<int> shipsCounts(4, 0);
    calculateOptimalLengthShips(std::stoi(options["size"]), shipsCounts);
    std::vector<int> temp;
    for (int i = 0; i < shipsCounts.size(); i++) {
        for (int j = 0; j < shipsCounts[i]; j++) {
            temp.push_back(i + 1);
        }
    }
    std::cout << std::endl;

    settings->setPlayerManager(temp);
    std::cout << "Player manager set with " << temp.size() << " ships." << std::endl;
    std::cout << std::endl;
    settings->setFieldSize(std::stoi(options["size"]));
    isFieldSizeSet = true;
}


bool CommonGameSettingsController::isFinishedSettings() const {
    return isFieldSizeSet;
}
