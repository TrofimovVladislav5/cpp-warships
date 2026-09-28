include(FetchContent)

# Once a dependency is on disk, leave it alone. Without this, every reconfigure runs a
# git update step against the network, which has failed here often enough to have wiped
# an already-populated source tree mid-build. Pinned tags never move, so there is nothing
# an update could usefully find.
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "" FORCE)

# Include libraries on testing
if (BUILD_TESTING)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG b514bdc898e2951020cbdca1304b75f5950d1f59
            GIT_SHALLOW TRUE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif ()

# Include JSON library
FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json)


# Include FTXUI for Terminal UI
FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v7.0.0
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ftxui)
