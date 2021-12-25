#include "Renderer.h"

Renderer::Renderer() {

}

Renderer::~Renderer() {

}

void Renderer::draw() {
    if (backgroundColor.isDirty()) {
        glClearColor(backgroundColor->r, backgroundColor->g, backgroundColor->b, backgroundColor->a);
        backgroundColor.clean();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    while (!draws.empty()) {
        renderCall(draws.front());
        draws.pop();
    }

}

void Renderer::setBackgroundColor(const glm::vec4& color) {
    backgroundColor = color;
}

void Renderer::drawArrays(const std::shared_ptr<VertexArray>& vao, PrimitiveMode mode, uint32_t count, uint32_t first) {
    preRender();
    vao->bind();
    glDrawArrays(static_cast<GLenum>(mode), first, count);
}

void Renderer::drawElements(const std::shared_ptr<VertexArray>& vao, PrimitiveMode mode, uint32_t count, uint32_t first) {
    preRender();
    vao->bind();
    glDrawElements(static_cast<GLenum>(mode), count, GL_UNSIGNED_INT, (const void*)first);
}

void Renderer::setForegroundColor(const glm::vec4& color) {
    currentForegroundColor = color;
}

void Renderer::setShader(const std::shared_ptr<ShaderProgram>& program) {
    currentShaderProgram = program;
}

void Renderer::preRender() {
    if (currentShaderProgram) {
        currentShaderProgram->use();
        currentShaderProgram->setFloat4("uColor", currentForegroundColor);
    }
}

void Renderer::queue(const RendererDraw& drawC) {
    draws.push(drawC);
}

void Renderer::renderCall(const RendererDraw& drawC) {
    setShader(drawC.shader);
    setForegroundColor(drawC.foregroundColor);

//    switch (drawC.drawData.renderMode) {
//    case RendererMode::Arrays:
//        drawArrays(drawC.drawData.vao, drawC.drawData.primitive, drawC.drawData.count, drawC.drawData.first);
//        break;
//    case RendererMode::Elements:
//        drawElements(drawC.drawData.vao, drawC.drawData.primitive, drawC.drawData.count, drawC.drawData.first);
//        break;
//    }

    drawMesh(drawC.mesh);
}