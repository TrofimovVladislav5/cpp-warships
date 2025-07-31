#include "../../../include/state_machine/substates/InitiateOngoingGameSubState.h"

#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>
#include <cpp_warships/input_parser/include/model/ParserCommandInfo.h>
#include <cpp_warships/input_parser/include/DefaultParserError.h>
#include <cpp_warships/input_parser/include/VoidParser.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>

#include "../../../include/PlaceShipController.h"
#include "../../../include/exceptions/ShipPlacementException.h"
#include "../../../include/state_machine/substates/BattleOngoingGameSubState.h"
#include "../../../include/state_machine/substates/PauseOngoingGameSubState.h"

namespace cpp_warships::application {

    void InitiateOngoingGameSubState::handleShipsShuffle(input_parser::ParsedOptions options) {
        playerPlaceController->placeShipsRandomly();
    }

    void InitiateOngoingGameSubState::handleConfirm(input_parser::ParsedOptions options) {
        if (playerPlaceController->allShipsPlaced()) {
            confirmed = true;
        } else {
            ViewHelper::errorOut("Cannot confirm ship placement when ships are not placed");
        }
    }

    void InitiateOngoingGameSubState::handlePause(input_parser::ParsedOptions options) {
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
        input_parser::ConfigCommandBuilder<void> commandBuilder;
        input_parser::DefaultParameterBuilder parameterBuilder;
        this->inputScheme = {
            {"shuffle", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&InitiateOngoingGameSubState::handleShipsShuffle, this))
                    .setDescription("Shuffles ships on the field")
                    .buildAndReset()
            )},
            {"add", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&PlaceShipController::addShip, playerPlaceController))
                    .setDescription("Adds a ship to the game field")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
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
            {"remove", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&PlaceShipController::removeShip, playerPlaceController))
                    .setDescription("Removes ship on field")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
                    .addParameter(
                        parameterBuilder
                            .addFlag("--cell")
                            .setValidator(std::regex("^[A-Z][0-9]{1,2}$"))
                            .setNecessary(true)
                            .buildAndReset()
                    )
                    .buildAndReset()
            )},
            {"confirm", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&InitiateOngoingGameSubState::handleConfirm, this))
                    .setDescription("Finish placing ships and start a game")
                    .buildAndReset()
            )},
            {"pause", input_parser::ParserCommandInfo(
               commandBuilder
                   .setCallback(TypesHelper::methodToFunction(&InitiateOngoingGameSubState::handlePause, this))
                   .setDescription("Pause the Placement of Ships")
                   .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
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
        input_parser::VoidParser parser(inputScheme, input_parser::DefaultParserError::CommandNotFoundError);

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
} // namespace cpp_warships::application

