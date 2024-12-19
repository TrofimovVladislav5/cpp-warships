#include <iostream>
#include "PauseOngoingGameSubState.h"

#include "ConfigCommandBuilder.h"
#include "DefaultParserError.h"
#include "StateMessages.h"
#include "TypesHelper.h"
#include "ongoing-game-substates/BattleOngoingGameSubState.h"
#include "../../library/ViewHelper.h"

void PauseOngoingGameSubState::handleResume(ParsedOptions options) {
    this->latestCommand = "resume";
}

PauseOngoingGameSubState::PauseOngoingGameSubState(SubStateContext &context)
    : OngoingGameSubState(context)
{
    ConfigCommandBuilder commandBuilder;
    this->inputScheme = {
        {"resume", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PauseOngoingGameSubState::handleResume, this))
                .setDescription("Continue the battle")
                .setDisplayError(DefaultParserError::CommandNotFoundError)
                .buildAndReset()

        )}
    };
}

void PauseOngoingGameSubState::openSubState() {
    ViewHelper::consoleOut("\n------------ Battle on paused ------------\n");
}

void PauseOngoingGameSubState::closeSubState() {
    ViewHelper::consoleOut("\n------------ Match resume ------------\n");
}

void PauseOngoingGameSubState::updateSubState() {
    StateMessages::awaitCommandMessage();
    std::getline(std::cin, latestCommand);
}

OngoingGameSubState* PauseOngoingGameSubState::transitToSubState() {
    if (latestCommand == "resume") {
        return new BattleOngoingGameSubState(context);
    }

    return nullptr;
}