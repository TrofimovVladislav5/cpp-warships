include_directories(./ src/)

add_library(utilities
        # Settings
        src/Initials.h
        # Helpers
        src/StateMessages.cpp src/StateMessages.h
        src/FieldCoordinateHelper.cpp src/FieldCoordinateHelper.h
        src/StringHelper.cpp src/StringHelper.h
        src/TypesHelper.cpp src/TypesHelper.h
        src/ViewHelper.cpp src/ViewHelper.h)

if (BUILD_TESTING)
    add_executable(utilities_tests tests/main.cpp)
    target_link_libraries(utilities_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(utilities_tests)
endif()

target_link_libraries(utilities PUBLIC cxx_setup)