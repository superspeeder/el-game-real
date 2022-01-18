#include "Renderer.h"
#include "Texture.h"
#include "Camera.h"

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
    if (!currentShaderProgram) { return; }
    currentShaderProgram->use();
    currentShaderProgram->setFloat4("uColor", currentForegroundColor);

    std::array<glm::uvec2, kMaximumTextures> texSizes;
    std::array<int, kMaximumTextures> texs;
    std::memset(texs.data(), 0, kMaximumTextures * sizeof(int));

    for (uint32_t uid = 0; uid < kMaximumTextures; uid++) {
        auto& tex = currentTextures[uid];
        if (tex) {
            tex->bind(uid);
//            std::string ind = "[";
//            ind += uid;
//            ind += "]";
//
//            currentShaderProgram->setUInt2("uTextureSize" + ind, tex->getTextureSize());
//            currentShaderProgram->setInt("uTexture" + ind, uid);
            texSizes[uid] = tex->getTextureSize();
            texs[uid] = uid;
        }
    }

    uint32_t utss_l = currentShaderProgram->getUniformLocation("uTextureSize");
    uint32_t utxs_l = currentShaderProgram->getUniformLocation("uTexture");

    uint32_t h = currentShaderProgram->getHandle();

    glUniform2uiv(utss_l, kMaximumTextures, reinterpret_cast<uint32_t*>(texSizes.data()));
    glUniform1iv(utxs_l, kMaximumTextures, texs.data());

    glm::mat4 vp = camera->getViewProjection();
    currentShaderProgram->setMatrix4("uViewProjection", vp);
}

void Renderer::queue(const RendererDraw& drawC) {
    draws.push(drawC);
}

void Renderer::renderCall(const RendererDraw& drawC) {
    setShader(drawC.shader);
    setForegroundColor(drawC.foregroundColor);

    currentTextures = drawC.textures;

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

void Renderer::drawMesh(const std::shared_ptr<Mesh>& mesh) {
    uint32_t count = mesh->getIndicesToRender();
    drawElements(mesh->getVertexArray(), mesh->getPrimitiveMode(), count);
}

void Renderer::setTexture(const std::shared_ptr<Texture>& tex, uint32_t unit) {
    if (unit >= kMaximumTextures) {
        spdlog::error("Cannot bind texture to unit {}. Maximum unit is {}", unit, kMaximumTextures);
        return;
    }

    currentTextures[unit] = tex;
}

void Renderer::setCamera(const std::shared_ptr<Camera>& cam) {
    camera = cam;
}

const std::shared_ptr<Camera>& Renderer::getCamera() {
    return camera;
}
