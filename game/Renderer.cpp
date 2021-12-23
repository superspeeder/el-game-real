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
}

void Renderer::setBackgroundColor(const glm::vec4& color) {
    backgroundColor = color;
}

void Renderer::drawArrays(VertexArray* vao, PrimitiveMode mode, uint32_t count, uint32_t first) {
    vao->bind();
    glDrawArrays(static_cast<GLenum>(mode), first, count);
}

void Renderer::drawElements(VertexArray* vao, PrimitiveMode mode, uint32_t count, uint32_t first) {
    vao->bind();
    glDrawElements(static_cast<GLenum>(mode), count, GL_UNSIGNED_INT, (const void*)first);
}
