#include "../../../include/state_machine/substates/BattleOngoingGameSubState.h"

#include <cpp_warships/utilities/include/StateMessages.h>
#include <cpp_warships/utilities/include/TypesHelper.h>
#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>
#include <cpp_warships/input_parser/include/DefaultParserError.h>
#include <cpp_warships/input_parser/include/VoidParser.h>

#include "../../../include/state_machine/substates/FinishOngoingGameSubState.h"
#include "../../../include/state_machine/substates/PauseOngoingGameSubState.h"
#include "../../../include/exceptions/BattleException.h"

namespace cpp_warships::application {

    OngoingGameSubState *BattleOngoingGameSubState::handleComputerWin() const {
        ViewHelper::consoleOut("Computer won the battle on round " + std::to_string(context->matchDTO->roundNumber));
        return new FinishOngoingGameSubState(context);
    }

    OngoingGameSubState * BattleOngoingGameSubState::handlePlayerWin() {
        ViewHelper::consoleOut("Player won round number " + std::to_string(context->matchDTO->roundNumber));
        context->matchDTO->roundNumber++;
        enemyPlaceController = new PlaceShipController(context->matchDTO, context->matchDTO->enemyManager);
        enemyPlaceController->placeShipsRandomly();
        context->matchDTO->enemyField->updateShipsCoordinateMap(
            enemyPlaceController->getCurrentField()->getShipsCoordinateMap()
        );

        return new BattleOngoingGameSubState(context);
    }

    void BattleOngoingGameSubState::handlePause(input_parser::ParsedOptions _) {
        isPaused = true;
    }


    BattleOngoingGameSubState::BattleOngoingGameSubState(SubStateContext* context)
        : OngoingGameSubState(context)
        , battleController(new BattleController(context->matchDTO))
        , enemyPlaceController(nullptr)
        , view(BattleView(context->matchDTO))
        , isPaused(false)
    {
        input_parser::ConfigCommandBuilder<void> commandBuilder;
        input_parser::DefaultParameterBuilder parameterBuilder;
        this->inputScheme = {
            {"attack", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&BattleController::battle, battleController))
                    .setDescription("Attack the opponent cell field")
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
            {"skill", input_parser::ParserCommandInfo(
                    commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&BattleController::applySkill, battleController))
                    .setDescription("Apply the skill")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
                    .buildAndReset()
            )},
            {"pause", input_parser::ParserCommandInfo(
                commandBuilder
                    .setCallback(TypesHelper::methodToFunction(&BattleOngoingGameSubState::handlePause, this))
                    .setDescription("Pause the battle")
                    .setDisplayError(input_parser::DefaultParserError::WrongFlagValueError)
                    .buildAndReset()
            )}
        };
    }

    BattleOngoingGameSubState::~BattleOngoingGameSubState() {
        delete battleController;
    }

    void BattleOngoingGameSubState::openSubState() {
        StateMessages::displayGreetingMessage("OngoingGame.Battle");
    }

    void BattleOngoingGameSubState::closeSubState() {
        ViewHelper::consoleOut("Battle ended");
        StateMessages::displayCloseMessage("OngoingGame.Battle");
    }

    void BattleOngoingGameSubState::updateSubState() {
        StateMessages::awaitCommandMessage();
        input_parser::VoidParser parser(this->inputScheme, input_parser::DefaultParserError::CommandNotFoundError);

        try {
            view.printBattleState();
            std::string input = context->getInputReader()->readCommand();
            parser.executedParse(input);
        } catch (const BattleException& exception) {
            exception.displayError();
        }
    }

    OngoingGameSubState* BattleOngoingGameSubState::transitToSubState() {
        if (isPaused) {
            return new PauseOngoingGameSubState(context);
        } else if (battleController->getBattleWinner() == BattleWinner::Computer) {
            return handleComputerWin();
        } else if (battleController->getBattleWinner() == BattleWinner::User) {
            return handlePlayerWin();
        }

        return nullptr;
    }
} // namespace cpp_warships::application
