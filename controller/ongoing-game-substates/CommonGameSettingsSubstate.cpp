#include "CommonGameSettingsSubstate.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/parser-builder/DefaultParameterBuilder.h"
#include "library/TypesHelper.h"
#include "library/defaults/DefaultParserError.h"
#include "library/StringHelper.h"
#include "InitiateOngoingGameSubstate.h"
#include "view/game-substates/CommonGameSettingsView.h"

CommonGameSettingsSubstate::CommonGameSettingsSubstate(StateContext& context) 
    : OngoingGameState(context)
{   
    ongoingGameView = new CommonGameSettingsView();
    controller = new CommonGameSettingsController(context);
    
    ConfigCommandBuilder commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"set", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&CommonGameSettingsController::handleGameFieldSize, controller))
                .setDescription("Choose a field size between 10 and 25")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--size")
                        .setValidator(std::regex("^(1[0-9]|2[0-5])$"))
                        .setNecessary(false)
                        .buildAndReset()
                )
                .buildAndReset()

        )},
        {"confirm", ParserCommandInfo (
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&CommonGameSettingsSubstate::handleConfirm, this))
                .setDescription("Confirm the game settings")
                .setDisplayError(DefaultParserError::CommandNotFoundError)
                .buildAndReset()
        )}
    };
}

void CommonGameSettingsSubstate::handleConfirm(ParsedOptions options) {
    this->latestCommand = "confirm";
}

CommonGameSettingsSubstate::~CommonGameSettingsSubstate() {
    delete controller;
}

void CommonGameSettingsSubstate::openSubstate() {
    ongoingGameView->displayOpenState();
    ongoingGameView->displayAvailableCommands(context.currentMatch);
}

void CommonGameSettingsSubstate::closeSubstate() {
    ongoingGameView->displayCloseState();
}

void CommonGameSettingsSubstate::updateSubstate() {
    std::string input;
    std::getline(std::cin, input);
    latestCommand = input;
    Parser parser(this->inputScheme, DefaultParserError::CommandNotFoundError);
    parser.executedParse(input);
}

OngoingGameState* CommonGameSettingsSubstate::transitToSubstate() {
    if (latestCommand == "confirm" && controller->isFinishedSettings()) {
        return new InitiateOngoingGameSubstate(context);
    }
    return nullptr;
}