# Compile files
add_library(warships_persistence
        # Where saves are kept
        include/application/persistence/SaveStorage.h
        include/application/persistence/FilesystemSaveStorage.h src/application/persistence/FilesystemSaveStorage.cpp
        include/application/persistence/MemorySaveStorage.h src/application/persistence/MemorySaveStorage.cpp
        # What is saved
        include/application/persistence/MatchSnapshot.h src/application/persistence/MatchSnapshot.cpp
        include/application/persistence/SaveArchive.h src/application/persistence/SaveArchive.cpp
        # How it is written
        include/application/persistence/serializers/SegmentJsonSerializer.h src/application/persistence/serializers/SegmentJsonSerializer.cpp
        include/application/persistence/serializers/ShipJsonSerializer.h src/application/persistence/serializers/ShipJsonSerializer.cpp
        include/application/persistence/serializers/BoardJsonSerializer.h src/application/persistence/serializers/BoardJsonSerializer.cpp
        include/application/persistence/serializers/MatchSettingsJsonSerializer.h src/application/persistence/serializers/MatchSettingsJsonSerializer.cpp
        include/application/persistence/serializers/MatchSnapshotJsonSerializer.h src/application/persistence/serializers/MatchSnapshotJsonSerializer.cpp
)

# Tests setup
if (BUILD_TESTING)
    add_executable(warships_persistence_tests tests/main.cpp)
    target_link_libraries(warships_persistence_tests PRIVATE GTest::gtest_main warships_persistence)
    include(GoogleTest)
    gtest_discover_tests(warships_persistence_tests)
endif()

# Expose settings
target_include_directories(warships_persistence
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
target_link_libraries(warships_persistence
        PUBLIC warships_core warships_flow serialization cxx_setup
        PRIVATE nlohmann_json::nlohmann_json
)
