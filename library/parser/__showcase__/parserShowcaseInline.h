#include <iostream>
#include <regex>

#include "library/TypesHelper.h"
#include "library/parser-builder/CommandBuildDirector.h"
#include "library/parser-builder/ConfigCommandBuilder.h"
#include "library/parser-builder/DefaultParameterBuilder.h"
#include "library/parser-builder/ParameterBuildDirector.h"
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

inline void runInlineParser() {
    TestClass test;

    auto* commandBuilder = new ConfigCommandBuilder();
    auto* parameterBuilder = new DefaultParameterBuilder();

    CommandBuildDirector commandDirector(commandBuilder);
    ParameterBuildDirector parameterDirector(parameterBuilder);

    commandDirector
        .buildBasicCommand(TypesHelper::methodToFunction(&TestClass::sayHello, &test), "The purpose of this function is to start an app")
        .setDisplayError(TypesHelper::methodToFunction(&TestClass::sayError, &test));

    parameterDirector.buildNecessary("--age", std::regex("^[1-9][0-9]*?$"));
    commandBuilder->addParameter(parameterBuilder->buildAndReset());
    parameterDirector.buildUnnecessary("--name", std::regex("^[A-Z][a-z]+(\\s[A-Z][a-z]+)?$"));
    commandBuilder->addParameter(parameterBuilder->buildAndReset());

    auto startCommand = commandBuilder->buildAndReset();

    commandDirector
        .buildBasicCommand(sayGoodbye, "The purpose of this function is to end an app")
        .setDisplayError(TypesHelper::methodToFunction(&TestClass::sayError, &test));

    parameterDirector.buildUnnecessary("--hello", std::regex("^[A-Z][a-z]+(\\s[A-Z][a-z]+)?$"));
    commandBuilder->addParameter(parameterBuilder->buildAndReset());

    auto endCommand = commandBuilder->buildAndReset();

    SchemeMap schemeMap = {
        {"start", ParserCommandInfo(startCommand)},
        {"end", ParserCommandInfo(endCommand)}
    };

    Parser parser(schemeMap, TypesHelper::methodToFunction(&TestClass::sayNotFound, &test));
    std::string input;
    std::getline(std::cin, input);
    parser.executedParse(input);
}