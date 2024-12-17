#pragma once
#include "game-states/GameState.h"

class MatchBuilder {
private:
    bool isLoaded;
    bool isLoadedFromTemplate;
    GameStateDTO* currentData;
    OngoingGameSubState* initializeNewMatch();
    OngoingGameSubState* loadSavedMatch();
public:
    explicit MatchBuilder();
    void newGame(bool fromTemplate = true);
    void loadSave(const std::string &filename);
    std::function<OngoingGameSubState*()> getStateBuilder();
};
