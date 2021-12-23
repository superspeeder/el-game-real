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
