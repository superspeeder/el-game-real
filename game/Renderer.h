#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "utils.hpp"

namespace colors {
    constexpr glm::vec4 BLACK{0,0,0,1};
    constexpr glm::vec4 WHITE{1,1,1,1};
    constexpr glm::vec4 RED{1,0,0,1};
    constexpr glm::vec4 GREEN{0,1,0,1};
    constexpr glm::vec4 BLUE{0,0,1,1};
    constexpr glm::vec4 YELLOW{1,1,0,1};
    constexpr glm::vec4 MAGENTA{1,0,1,1};
    constexpr glm::vec4 CYAN{0,1,1,1};
}

class Renderer {
public:

    Renderer();
    ~Renderer();

    void draw();

    void setBackgroundColor(const glm::vec4& color);

private:
    
    dirtyable<glm::vec4> backgroundColor = colors::BLACK;
};
