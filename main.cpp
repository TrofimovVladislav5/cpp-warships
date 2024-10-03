#include <iostream>

#include "view/parser/Parser.h"

void sayHello(std::map<std::string, std::string> args) {
    printf("Hello, world!\n");
}

std::string sayGoodbye(std::map<std::string, std::string> args) {
    printf("Goodbye!\n");
}

int main(){
    // GameController controller;
    // controller.run();

    std::map<std::string, ParserCommandInfo<void>> schemeMap = {
        {"start", ParserCommandInfo<void>(ParserCommandInfoConfig<std::string>(
            "The purpose of this function is to start an app",
            {
                ParserParameter<std::string>({"--age"}, std::regex("(-?\\d+)")),
                ParserParameter<std::string>({"--name"}, std::regex("^[A-Z][a-z]+\\s[A-Z][a-z]+$")),
            },
            sayHello
            ))},
        {"end", ParserCommandInfo<void>(ParserCommandInfoConfig<std::string>(
            "The purpose of this function is to end an app",
            {
                ParserParameter<std::string>({"--hello"}, std::regex("(-?\\d+)"))
            },
            sayGoodbye
            ))}
    };

    Parser<void> parser(schemeMap);
    std::string input;
    std::getline(std::cin, input);
    parser.parse(input);

    return 0;
}
