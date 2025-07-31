#pragma once

#include <cpp_warships/input_parser/include/model/Parser.h>

#include "state_machine/states/StateContext.h"

namespace cpp_warships::application {

    class MatchSettingsController {
    private:
        std::map<int, int> calculateOptimalLengthShips(int fieldSize);

    public:
        explicit MatchSettingsController();
        ~MatchSettingsController();
        MatchSettings* createMatchSettings(int fieldSize);
    };
} // namespace cpp_warships::application