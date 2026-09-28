# Compile files
add_library(platform
        # Every branch on the host operating system, and nothing else
        include/platform/OSBrancher.h src/platform/OSBrancher.cpp)

# Tests setup
if (BUILD_TESTING)
    add_executable(platform_tests tests/OSBrancherTests.cpp)
    target_link_libraries(platform_tests PRIVATE GTest::gtest_main platform)
    include(GoogleTest)
    gtest_discover_tests(platform_tests)
endif()

# Expose settings
target_include_directories(platform
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(platform PUBLIC cxx_setup)
