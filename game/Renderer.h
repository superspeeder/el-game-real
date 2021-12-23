#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "utils.hpp"
#include "Buffer.h"

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


enum class PrimitiveMode : GLenum {
    Points = GL_POINTS,

    Lines = GL_LINES,
    LinesAdjacency = GL_LINES_ADJACENCY,
    LineStrip = GL_LINE_STRIP,
    LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
    LineLoop = GL_LINE_LOOP,

    Triangles = GL_TRIANGLES,
    TrianglesAdjacency = GL_TRIANGLES_ADJACENCY,
    TriangleStrip = GL_TRIANGLE_STRIP,
    TriangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
    TriangleFan = GL_TRIANGLE_FAN,

    Patches = GL_PATCHES
};


class Renderer {
public:

    Renderer();
    ~Renderer();

    void draw();

    void drawArrays(VertexArray* vao, PrimitiveMode mode, uint32_t count, uint32_t first = 0);
    void drawElements(VertexArray* vao, PrimitiveMode mode, uint32_t count, uint32_t first = 0);

    void setBackgroundColor(const glm::vec4& color);

private:
    
    dirtyable<glm::vec4> backgroundColor = colors::BLACK;
};
