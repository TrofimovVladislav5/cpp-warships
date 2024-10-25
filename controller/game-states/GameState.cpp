#include "GameState.h"
#include "model/StateContext.h"

GameState::GameState(StateContext& context) 
    : context(context)
    , view(nullptr)
{}

GameState::~GameState() {
    if (view) 
        delete view;
}