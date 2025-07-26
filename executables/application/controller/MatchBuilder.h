#pragma once

#include <functional>
#include <utility>

#include "game_states/GameState.h"

namespace cpp_warships::application {

    struct Settings {
        Settings(std::map<int, int> shipsCount, int fieldSize)
            : shipsCount(std::move(shipsCount)), fieldSize(fieldSize) {}

        std::map<int, int> shipsCount;
        int fieldSize;
    };

    class MatchBuilder {
    private:
        const Settings defaultSettings = {{{1, 4}, {2, 3}, {3, 2}, {4, 1}}, 10};
        bool isLoaded;
        bool isLoadedFromTemplate;
        GameStateDTO* currentData;
        input_reader::InputReader<>* reader;
        OngoingGameSubState* initializeNewMatch();
        OngoingGameSubState* loadSavedMatch();

    public:
        explicit MatchBuilder(input_reader::InputReader<>* reader);
        void newGame(bool fromTemplate = true);
        bool loadSave(const std::string& filename);
        void printBattleScreenshot();
        std::function<OngoingGameSubState*()> getStateBuilder();
    };
} // namespace cpp_warships::application