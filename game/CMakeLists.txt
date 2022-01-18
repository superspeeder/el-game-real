cmake_minimum_required (VERSION 3.8)


add_executable(game "game.cpp" "game.hpp" "Window.h" "Window.cpp" "Renderer.h" "Renderer.cpp" "utils.hpp" "Buffer.h" "Buffer.cpp" "Shader.h" "Shader.cpp" "Mesh.h" "Mesh.cpp" "fwd.h" "SpriteBatch.h" "SpriteBatch.cpp" "Texture.h" "Texture.cpp" "stb_image.cpp" "Camera.h" "Camera.cpp" "event.cpp" "event.hpp" "Font.h" "Font.cpp" "Framebuffer.h" "Framebuffer.cpp" "AssetManager.h" "AssetManager.cpp")

target_include_directories(game PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(game PRIVATE glfw glad::glad glm::glm rapidjson spdlog::spdlog Freetype::Freetype ${Boost_LIBRARIES})

target_compile_definitions(game PUBLIC 
    $<$<CONFIG:Debug>:GAME_DEBUG>
    $<$<CONFIG:Release>:GAME_RELEASE>

    $<$<CONFIG:RelWithDebInfo>:GAME_DEBUG>
    $<$<CONFIG:MinSizeRel>:GAME_RELEASE>)
