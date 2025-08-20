# Compile files
add_library(game_saves
        # Main Entities
        src/source.cpp
        include/ISerializable.h
        include/ISerializer.h
        include/SerializerAggregator.h
        # Custom Exceptions
        include/exceptions/SerializationException.h
        include/exceptions/DeserializationException.h
        include/exceptions/InterpretationException.h
        # Helpers
        include/helpers/TupleBuilder.h
        include/helpers/serializers/JsonStringSerializer.h
        include/helpers/type_converters/StringTypeConverter.h
        # Example Classes
        include/example/TestClass.h
        include/example/ImplicitTestClass.h
)

# Tests setup
if (BUILD_TESTING)
    add_executable(game_saves_tests tests/main.cpp)
    target_link_libraries(game_saves_tests PRIVATE GTest::gtest_main game_saves)
    include(GoogleTest)
    gtest_discover_tests(game_saves_tests)
endif()

# Expose settings
target_include_directories(game_saves
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(game_saves PUBLIC cxx_setup)