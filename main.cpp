#include <iostream>
#include <regex>

#include "library/TypesHelper.h"
#include "view/parser/Parser.h"

void sayHello(ParsedOptions args) {
    const std::string name = args["name"].empty() ? "stranger" : args["name"];

    std::cout << "Hello, " << name << "!\n";
    if (!args["age"].empty()) std::cout << "Age: " << args["age"] << "\n";
}

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
};


//TODO: add aliases (?)
//TODO: add dynamic types with templates (*)

int main(){
    // GameController controller;
    // controller.run();

    TestClass test;

    SchemeMap schemeMap = {
        {"start", ParserCommandInfo({
            "The purpose of this function is to start an app",
            {
                ParserParameter({"--age"}, std::regex("^[1-9][0-9]*?$")),
                ParserParameter({"--name"}, std::regex("^[A-Z][a-z]+(\\s[A-Z][a-z]+)?$"))
            },
            TypesHelper::methodToFunction(&TestClass::sayHello, &test)
        })},
        {"end", ParserCommandInfo({
            "The purpose of this function is to end an app",
            {
                ParserParameter({"--hello"}, std::regex("^[1-9][0-9]*?$"))
            },
            sayGoodbye
        })},
    };

    Parser parser(schemeMap);
    std::string input;
    std::getline(std::cin, input);
    parser.executedParse(input);

    return 0;
}
