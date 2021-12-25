#pragma once

#include "fwd.h"
#include "utils.hpp"

#include "Buffer.h"
#include "Shader.h"
#include "Mesh.h"

#include <variant>
#include <queue>

struct RendererDraw {
    std::shared_ptr<Mesh> mesh;

    std::shared_ptr<ShaderProgram> shader;
    glm::vec4 foregroundColor = colors::WHITE;
};

class Renderer {
public:

    Renderer();
    ~Renderer();

    void draw();

    void drawArrays(const std::shared_ptr<VertexArray>& vao, PrimitiveMode mode, uint32_t count, uint32_t first = 0);
    void drawElements(const std::shared_ptr<VertexArray>& vao, PrimitiveMode mode, uint32_t count, uint32_t first = 0);

    void setBackgroundColor(const glm::vec4& color);
    void setForegroundColor(const glm::vec4& color);
    void setShader(const std::shared_ptr<ShaderProgram>& program);

    void queue(const RendererDraw& drawC);

    void drawMesh(const std::shared_ptr<Mesh>& mesh)

private:
    
    void renderCall(const RendererDraw& drawC);
    void preRender();

    std::shared_ptr<ShaderProgram> currentShaderProgram;
    glm::vec4 currentForegroundColor;

    dirtyable<glm::vec4> backgroundColor = colors::BLACK;
    std::queue<RendererDraw> draws;
    
};

