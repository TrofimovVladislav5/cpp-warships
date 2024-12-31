#include "InitiateOngoingGameSubState.h"
#include "../PlaceShipController.h"
#include "BattleOngoingGameSubState.h"
#include "PauseOngoingGameSubState.h"
#include "StateMessages.h"
#include "ViewHelper.h"
#include "exceptions/ShipPlacementException.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/parser/ParserCommandInfo.h"
#include "library/defaults/DefaultParserError.h"
#include "library/TypesHelper.h"
#include "library/parser-builder/DefaultParameterBuilder.h"
#include "void/VoidParser.h"


void InitiateOngoingGameSubState::handleShipsShuffle(ParsedOptions options) {
    playerPlaceController->placeShipsRandomly();
}

void InitiateOngoingGameSubState::handleConfirm(ParsedOptions options) {
    if (playerPlaceController->allShipsPlaced()) {
        confirmed = true;
    } else {
        ViewHelper::errorOut("Cannot confirm ship placement when ships are not placed");
    }
}

void InitiateOngoingGameSubState::handlePause(ParsedOptions options) {
    latestCommand = "pause";
}

InitiateOngoingGameSubState::InitiateOngoingGameSubState(SubStateContext* context)
    : OngoingGameSubState(context)
    , playerPlaceController(new PlaceShipController(context->matchDTO, context->matchDTO->playerManager))
    , enemyPlaceController(new PlaceShipController(context->matchDTO, context->matchDTO->enemyManager))
    , placeControllerView(new PlaceShipControllerView(playerPlaceController))
    , confirmed(false)
{
    context->matchDTO->lastSubState = "InitiateOngoingGameSubState";
    ConfigCommandBuilder<void> commandBuilder;
    DefaultParameterBuilder parameterBuilder;
    this->inputScheme = {
        {"shuffle", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&InitiateOngoingGameSubState::handleShipsShuffle, this))
                .setDescription("Shuffles ships on the field")
                .buildAndReset()
        )},
        {"add", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PlaceShipController::addShip, playerPlaceController))
                .setDescription("Adds a ship to the game field")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--length")
                        .setValidator(std::regex("^[1-4]$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .addParameter (
                    parameterBuilder
                        .addFlag("--direction")
                        .setValidator(std::regex("^(horizontal|vertical)$"))
                        .setNecessary(true)
                        .buildAndReset()
                )

                .addParameter (
                    parameterBuilder
                        .addFlag("--cell")
                        .setValidator(std::regex("^[A-Z][0-9]{1,2}$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
        )},
        {"remove", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&PlaceShipController::removeShip, playerPlaceController))
                .setDescription("Removes ship on field")
                .setDisplayError(DefaultParserError::WrongFlagValueError)
                .addParameter(
                    parameterBuilder
                        .addFlag("--cell")
                        .setValidator(std::regex("^[A-Z][0-9]{1,2}$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
        )},
        {"confirm", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&InitiateOngoingGameSubState::handleConfirm, this))
                .setDescription("Finish placing ships and start a game")
                .buildAndReset()
        )},
        {"pause", ParserCommandInfo(
           commandBuilder
               .setCallback(TypesHelper::methodToFunction(&InitiateOngoingGameSubState::handlePause, this))
               .setDescription("Pause the Placement of Ships")
               .setDisplayError(DefaultParserError::WrongFlagValueError)
               .buildAndReset()
        )}
    };
}

InitiateOngoingGameSubState::~InitiateOngoingGameSubState() {
    delete playerPlaceController;
    delete enemyPlaceController;
    delete placeControllerView;
}

void InitiateOngoingGameSubState::openSubState() {
    StateMessages::displayGreetingMessage("New Game Initiation");
}

void InitiateOngoingGameSubState::updateSubState() {
    StateMessages::awaitCommandMessage();
    VoidParser parser(inputScheme, DefaultParserError::CommandNotFoundError);

    try {
        std::string input = context->getInputReader()->readCommand();
        parser.executedParse(input);
        placeControllerView->displayCurrentField();
        placeControllerView->displayShipsLeft();
    } catch (const ShipPlacementException& exception) {
        exception.displayError();
    }
}

void InitiateOngoingGameSubState::closeSubState() {
}

OngoingGameSubState* InitiateOngoingGameSubState::transitToSubState() {
    if (confirmed && playerPlaceController->allShipsPlaced()) {
        enemyPlaceController->placeShipsRandomly();
        context->matchDTO->playerManager = playerPlaceController->getCurrentManager();
        context->matchDTO->playerField = playerPlaceController->getCurrentField();
        context->matchDTO->enemyField = enemyPlaceController->getCurrentField();
        context->matchDTO->enemyManager = enemyPlaceController->getCurrentManager();
        context->matchDTO->playerSkillManager = new SkillManager(
            enemyPlaceController->getCurrentField(),
            context->matchDTO->settings
        );

        return new BattleOngoingGameSubState(context);
    } else if (latestCommand == "pause") {
        return new PauseOngoingGameSubState(context);
    }

    return nullptr;
}