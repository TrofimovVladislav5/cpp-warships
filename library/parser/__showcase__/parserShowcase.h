#include <iostream>
#include <regex>

#include "library/TypesHelper.h"
#include "library/ViewHelper.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/parser-builder/DefaultParameterBuilder.h"
#include "library/parser/Parser.h"
#include "library/parser/ParserCommandInfo.h"

void sayGoodbye(ParsedOptions args) {
    std::cout << "Goodbye!\n";
}

class TestClass {
public:
    void sayHello(ParsedOptions args) {
        const std::string name = args["name"].empty() ? "stranger" : args["name"];

        std::cout << "Hello, " << name << "!\n";
        if (!args["age"].empty()) std::cout << "Age: " << args["age"] << "\n";
    }

    void sayError(ParsedOptions args) {
        std::cout << "Error!\n";
    }

    void sayNotFound(ParsedOptions args) {
        std::cout << "Command not found or necessary flags are not provided!\n";
    }
};

//TODO: add help command
//TODO: (?) add aliases

void printHelpTest(ParsedOptions options) {
    ViewHelper::consoleOut("Hello, world!");
}

inline void runParser(){
    TestClass test;

    ConfigCommandBuilder commandBuilder;
    DefaultParameterBuilder parameterBuilder;

    SchemeMap schemeMap = {
        {"start", ParserCommandInfo(
            commandBuilder
                .setCallback(TypesHelper::methodToFunction(&TestClass::sayHello, &test))
                .setDescription("The purpose of this function is to start an app")
                .setDisplayError(TypesHelper::methodToFunction(&TestClass::sayError, &test))
                .addParameter(
                    parameterBuilder
                        .addFlag("--age")
                        .setValidator(std::regex("^[1-9][0-9]*?$"))
                        .setNecessary(true)
                        .buildAndReset()
                )
                .addParameter(
                    parameterBuilder
                        .addFlag("--name")
                        .setValidator(std::regex("^[A-Z][a-z]+(\\s[A-Z][a-z]+)?$"))
                        .buildAndReset()
                )
                .buildAndReset()
            )
        },
        {"end", ParserCommandInfo(
            commandBuilder
                .setCallback(sayGoodbye)
                .setDescription("The purpose of this function is to end an app")
                .addParameter(
                    parameterBuilder
                        .addFlag("--hello")
                        .setValidator(std::regex("^[1-9][0-9]*?$"))
                        .buildAndReset()
                )
                .buildAndReset()
            )
        },
    };

    Parser parser(schemeMap, TypesHelper::methodToFunction(&TestClass::sayNotFound, &test));
    std::string input;
    std::getline(std::cin, input);
    parser.executedParse(input);
}