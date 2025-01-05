include_directories(./ ./src ./src/builder ./src/command ./src/command/helpers ./src/void ./src/void/helpers)

add_library(input_parser
        # Parser
        src/Command.h
        src/Parser.h
        src/ParserCommand.h
        src/ParserCommandInfo.h
        src/ParserParameter.h
        # Void Parser Implementation
        src/void/VoidParser.h src/void/VoidParser.cpp
        src/void/helpers/DefaultHelp.h src/void/helpers/DefaultHelp.cpp
        src/void/helpers/DefaultParserError.h src/void/helpers/DefaultParserError.cpp
        # Command Parser Implementation
        src/command/CommandParser.h src/command/CommandParser.cpp
        src/command/helpers/ErrorCommand.h src/command/helpers/ErrorCommand.cpp
        src/command/helpers/HelpCommand.h src/command/helpers/HelpCommand.cpp
        src/command/helpers/ArgumentsErrorCommand.h src/command/helpers/ArgumentsErrorCommand.cpp
        # Builder
        src/builder/ConfigCommandBuilder.h
        src/builder/ParserCommandBuilder.h
        src/builder/DefaultParameterBuilder.h
        src/builder/ParserParameterBuilder.h
        src/builder/CommandBuildDirector.h
        src/builder/ParameterBuildDirector.h
        src/builder/ParserBuilderDirector.h)

if (BUILD_TESTING)
    add_executable(input_parser_tests tests/main.cpp)
    target_link_libraries(input_parser_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(input_parser_tests)
endif()

target_link_libraries(input_parser PUBLIC cxx_setup utilities)