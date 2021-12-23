cmake_minimum_required (VERSION 3.8)


add_executable(game "game.cpp" "game.hpp" "Window.h" "Window.cpp")

target_include_directories(game PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(game PRIVATE glfw glad::glad glm::glm)
