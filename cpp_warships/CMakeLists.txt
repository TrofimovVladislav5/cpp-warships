# Reusable libraries, owing nothing to this game.
add_subdirectory(utilities)
add_subdirectory(serialization)
add_subdirectory(input_parser)
add_subdirectory(input_reader)

# The game itself, layered from the rules outwards.
add_subdirectory(application)
