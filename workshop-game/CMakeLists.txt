set (GAME_SOURCE_FILES
    text.h
    text.cpp
    draw_game.cpp
    draw_game.h
    game_object.cpp
    game_object.h
    character.h
    character.cpp
    block.h
    block.cpp
    bullet.h
    bullet.cpp
    imgui_impl_glfw_game.cpp
    imgui_impl_glfw_game.h
    imgui_impl_opengl3_game.cpp
    imgui_impl_opengl3_game.h
    main.cpp
)

add_executable(game ${GAME_SOURCE_FILES})
target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(game PUBLIC box2d glfw imgui sajson glad)
set_target_properties(game PROPERTIES
	CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# message(STATUS "runtime = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
# message(STATUS "binary = ${CMAKE_CURRENT_BINARY_DIR}")

# Copy font files, etc
add_custom_command(
        TARGET game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/../testbed/data/
                ${CMAKE_CURRENT_BINARY_DIR}/data/)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GAME_SOURCE_FILES})
