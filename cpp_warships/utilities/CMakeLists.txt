include_directories(./ src/)

add_library(utilities
        # Settings
        include/Initials.h
        # Helpers
        src/StateMessages.cpp include/StateMessages.h
        src/FieldCoordinateHelper.cpp include/FieldCoordinateHelper.h
        src/StringHelper.cpp include/StringHelper.h
        src/TypesHelper.cpp include/TypesHelper.h
        src/ViewHelper.cpp include/ViewHelper.h)

if (BUILD_TESTING)
    add_executable(utilities_tests tests/main.cpp)
    target_link_libraries(utilities_tests PRIVATE GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(utilities_tests)
endif()

# Expose settings
target_include_directories(utilities
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(utilities PUBLIC cxx_setup)