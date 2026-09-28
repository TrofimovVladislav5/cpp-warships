# Compile files
add_executable(cpp_warships
        # Putting the parts together is this layer's whole job
        include/build_save_archive.h src/build_save_archive.cpp
        include/build_game.h src/build_game.cpp
        include/build_queries.h src/build_queries.cpp
        include/build_event_pipeline.h src/build_event_pipeline.cpp
        include/build_shell.h src/build_shell.cpp
        include/build_renderers.h src/build_renderers.cpp
        include/run_game.h src/run_game.cpp
        # The way in
        main.cpp
)

# Expose settings
target_include_directories(cpp_warships
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies
# warships_head_tui carries warships_head, and that carries the layers below it,
# so naming any of them again here only makes the linker warn about duplicates.
target_link_libraries(cpp_warships
        PRIVATE warships_head_tui
        PRIVATE cxx_setup
)
