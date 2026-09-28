# The layers of the game, innermost first. Each one may only reach inwards:
# core knows nobody, and head is the only layer that draws.
add_subdirectory(core)
add_subdirectory(flow)
add_subdirectory(persistence)
add_subdirectory(model)
add_subdirectory(head)
