#include <iostream>

#include "view/parser/Parser.h"

void sayHello(std::map<std::string, std::string> args) {
    std::cout << "Hello, world!\n";
    std::cout << "Age: " << args["--age"] << "\n";
    std::cout << "Name: " << args["--name"] << "\n";
}

void sayGoodbye(std::map<std::string, std::string> args) {
    std::cout << "Goodbye!\n";
}

int main(){
    // GameController controller;
    // controller.run();

    std::map<std::string, ParserCommandInfo> schemeMap = {
        {"start", ParserCommandInfo(ParserCommandInfoConfig(
            "The purpose of this function is to start an app",
            {
                ParserParameter({"--age"}, std::regex("^0$|^[1-9][0-9]*$")),
                ParserParameter({"--name"}, std::regex("^[A-Z][a-z]+\\s[A-Z][a-z]+$")),
            },
            sayHello
            ))},
        {"end", ParserCommandInfo(ParserCommandInfoConfig(
            "The purpose of this function is to end an app",
            {
                ParserParameter({"--hello"}, std::regex("(-?\\d+)"))
            },
            sayGoodbye
            ))}
    };

    Parser parser(schemeMap);
    std::string input;
    std::getline(std::cin, input);
    auto result = parser.parse(input);
    result.first(result.second);

    return 0;
}
