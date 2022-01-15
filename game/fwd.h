#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>


// Forwards

//-Renderer.h
class Renderer;

struct RendererDraw;

struct SimpleTexturedQuad;

//-Mesh.h
class Mesh;

//-Buffer.h
class VertexBuffer;
class IndexBuffer;
class VertexArray;

//-utils.hpp
struct SimpleFunctionalGradient;
struct CosineWave;

template<typename T>
class dirty_this;

class DirtyableType;

template<typename T>
struct dirtyable;

//-Shader.h
class Shader;
class ShaderProgram;

//-Window.h
class Window;
struct WindowSettings;

//-Texture.h
class Texture;
struct TextureParams;

//-Camera.h
class Camera;

//-game.hpp
class Game;

//-Framebuffer.h
class Framebuffer;

//-Font.h
class FontFamily;
class Font;
class RenderableText;

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

enum class FramebufferTarget {
    Read = GL_READ_FRAMEBUFFER,
    Draw = GL_DRAW_FRAMEBUFFER,
    Both = GL_FRAMEBUFFER
};

enum class FramebufferAttachment {
    Color0 = GL_COLOR_ATTACHMENT0,
    Color1 = GL_COLOR_ATTACHMENT1,
    Color2 = GL_COLOR_ATTACHMENT2,
    Color3 = GL_COLOR_ATTACHMENT3,
    Color4 = GL_COLOR_ATTACHMENT4,
    Color5 = GL_COLOR_ATTACHMENT5,
    Color6 = GL_COLOR_ATTACHMENT6,
    Color7 = GL_COLOR_ATTACHMENT7,
    Color8 = GL_COLOR_ATTACHMENT8,
    Color9 = GL_COLOR_ATTACHMENT9,
    Color10 = GL_COLOR_ATTACHMENT10,
    Color11 = GL_COLOR_ATTACHMENT11,
    Color12 = GL_COLOR_ATTACHMENT12,
    Color13 = GL_COLOR_ATTACHMENT13,
    Color14 = GL_COLOR_ATTACHMENT14,
    Color15 = GL_COLOR_ATTACHMENT15,
    Color16 = GL_COLOR_ATTACHMENT16,
    Color17 = GL_COLOR_ATTACHMENT17,
    Color18 = GL_COLOR_ATTACHMENT18,
    Color19 = GL_COLOR_ATTACHMENT19,
    Color20 = GL_COLOR_ATTACHMENT20,
    Color21 = GL_COLOR_ATTACHMENT21,
    Color22 = GL_COLOR_ATTACHMENT22,
    Color23 = GL_COLOR_ATTACHMENT23,
    Color24 = GL_COLOR_ATTACHMENT24,
    Color25 = GL_COLOR_ATTACHMENT25,
    Color26 = GL_COLOR_ATTACHMENT26,
    Color27 = GL_COLOR_ATTACHMENT27,
    Color28 = GL_COLOR_ATTACHMENT28,
    Color29 = GL_COLOR_ATTACHMENT29,
    Color30 = GL_COLOR_ATTACHMENT30,
    Color31 = GL_COLOR_ATTACHMENT31,
    Depth = GL_DEPTH_ATTACHMENT,
    Stencil = GL_STENCIL_ATTACHMENT,
    DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT
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
