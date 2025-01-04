include_directories(./ src/builder src/command src/command/helpers src/void)

add_library(input_parser
        # Parser
        src/Parser.h
        src/ParserCommand.h
        src/ParserCommandInfo.h
        src/ParserParameter.h
        # Parser Implementations
        src/void/VoidParser.h src/void/VoidParser.cpp
        src/command/CommandParser.h src/command/CommandParser.cpp
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


target_link_libraries(input_parser PUBLIC cxx_setup)