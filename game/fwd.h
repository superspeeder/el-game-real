#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>


// Forwards
class Renderer;

struct RendererDraw;

class Mesh;

class VertexBuffer;
class IndexBuffer;
class VertexArray;

struct SimpleFunctionalGradient;
struct CosineWave;

template<typename T>
class dirty_this;

class DirtyableType;

template<typename T>
struct dirtyable;

class Shader;
class ShaderProgram;

class Window;
struct WindowSettings;

class Texture;
struct TextureParams;

class Renderable;

// Enums
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

enum class RendererMode {
    Arrays, Elements
};

enum class BufferMode : GLenum {
    StaticDraw = GL_STATIC_DRAW,
    StaticRead = GL_STATIC_READ,
    StaticCopy = GL_STATIC_COPY,

    DynamicDraw = GL_DYNAMIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicCopy = GL_DYNAMIC_COPY,

    StreamDraw = GL_STREAM_DRAW,
    StreamRead = GL_STREAM_READ,
    StreamCopy = GL_STREAM_COPY
};

// Constants
namespace colors {
    constexpr glm::vec4 BLACK{ 0,0,0,1 };
    constexpr glm::vec4 WHITE{ 1,1,1,1 };
    constexpr glm::vec4 RED{ 1,0,0,1 };
    constexpr glm::vec4 GREEN{ 0,1,0,1 };
    constexpr glm::vec4 BLUE{ 0,0,1,1 };
    constexpr glm::vec4 YELLOW{ 1,1,0,1 };
    constexpr glm::vec4 MAGENTA{ 1,0,1,1 };
    constexpr glm::vec4 CYAN{ 0,1,1,1 };
}
