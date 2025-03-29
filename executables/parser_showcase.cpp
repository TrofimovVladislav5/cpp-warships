#include <cpp_warships/utilities/include/ViewHelper.h>
#include <cpp_warships/input_parser/include/VoidParser.h>
#include <cpp_warships/input_parser/include/builder/ConfigCommandBuilder.h>
#include <cpp_warships/input_parser/include/builder/DefaultParameterBuilder.h>

using namespace cpp_warships::input_parser;

void handleLoad(ParsedOptions options) {
    ViewHelper::consoleOut("Load");
}

void handleNew(ParsedOptions options) {
    ViewHelper::consoleOut("New");
}

void handleInfo(ParsedOptions options) {
    ViewHelper::consoleOut("Info");
}

void handleList(ParsedOptions options) {
    ViewHelper::consoleOut("List");
}

void handleConfirm(ParsedOptions options) {
    ViewHelper::consoleOut("Confirm");
}

int main() {
    ConfigCommandBuilder<void> commandBuilder;
    DefaultParameterBuilder parameterBuilder;

    SchemeMap<void> inputScheme = {
        {"load", ParserCommandInfo(
            commandBuilder
                .setCallback(handleLoad)
                .setDescription("Load a game from a file")
                .addParameter(
                    parameterBuilder
                        .addFlag("--filename")
                        .setDescription("Specify the file name to load the game from. Make sure it's a .json file")
                        .setValidator(std::regex("^.*\\.json$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .buildAndReset()
            )
        },
        {"new", ParserCommandInfo(
            commandBuilder
                .setCallback(handleNew)
                .setDescription("Start new game from scratch")
                .addParameter(
                    parameterBuilder
                        .addFlag("--default")
                        .setDescription("Start game with default settings and skip the initialization phase (true/false")
                        .setNecessary(false)
                        .setValidator(std::regex("^(true|false)$"))
                        .buildAndReset()
                    )
                .buildAndReset()
            )
        },
        {"info", ParserCommandInfo(
            commandBuilder
                .setCallback(handleInfo)
                .setDescription("Print the latest screenshot from currently handled save")
                .buildAndReset()
            )
        },
        {"list", ParserCommandInfo(
            commandBuilder
                .setCallback(handleList)
                .setDescription("List all available saves")
                .addParameter(
                    parameterBuilder
                        .addFlag("--filename")
                        .setNecessary(false)
                        .setDescription("Specify path to the directory with saves")
                        .setValidator(std::regex("^.*$"))
                        .buildAndReset()
                )
                .buildAndReset()
            )
        },
        {"confirm", ParserCommandInfo(
            commandBuilder
                .setCallback(handleConfirm)
                .setDescription("Confirm the initialization phase and start the game")
                .buildAndReset()
            )
        }
    };

    while (true) {
        std::string input;
        std::getline(std::cin, input);
        VoidParser parser(inputScheme);
        parser.executedParse(input);
        std::cout << "Enter new command: ";
    }
}
