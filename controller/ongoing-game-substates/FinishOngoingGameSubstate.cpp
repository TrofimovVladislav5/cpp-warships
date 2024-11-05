#include "FinishOngoingGameSubstate.h"

#include "ViewHelper.h"

FinishOngoingGameSubstate::FinishOngoingGameSubstate(StateContext& context)
    :   OngoingGameState(context)

{
    ViewHelper::consoleOut("FinishGameState");
}
