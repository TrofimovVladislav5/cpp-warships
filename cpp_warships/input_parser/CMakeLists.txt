# Compile files
add_library(input_parser
        # Parser
        include/command/Command.h
        include/model/Parser.h
        include/command/ParserCommand.h
        include/model/ParserCommandInfo.h
        include/model/ParserParameter.h
        # Void Parser Implementation
        include/VoidParser.h src/VoidParser.cpp
        include/DefaultHelp.h src/DefaultHelp.cpp
        include/DefaultParserError.h src/DefaultParserError.cpp
        # Command Parser Implementation
        include/CommandParser.h src/CommandParser.cpp
        include/command/ErrorCommand.h src/command/ErrorCommand.cpp
        include/command/HelpCommand.h src/command/HelpCommand.cpp
        include/command/ArgumentsErrorCommand.h src/command/ArgumentsErrorCommand.cpp
        # Builder
        include/builder/ConfigCommandBuilder.h
        include/builder/ParserCommandBuilder.h
        include/builder/DefaultParameterBuilder.h
        include/builder/ParserParameterBuilder.h
        include/builder/CommandBuildDirector.h
        include/builder/ParameterBuildDirector.h
        include/builder/ParserBuilderDirector.h)

# Tests setup
if (BUILD_TESTING)
    add_executable(input_parser_tests tests/main.cpp)
    target_link_libraries(input_parser_tests PRIVATE GTest::gtest_main input_parser)
    include(GoogleTest)
    gtest_discover_tests(input_parser_tests)
endif()

# Expose settings
target_include_directories(input_parser
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(input_parser PUBLIC utilities cxx_setup)