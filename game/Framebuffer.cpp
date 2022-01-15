#include "Framebuffer.h"
#include "Texture.h"

using namespace framebuffer_constants;

void Framebuffer::bindDefault(FramebufferTarget target) {
	glBindFramebuffer(static_cast<GLenum>(target), kDefaultFramebuffer);
}

Framebuffer::Framebuffer() {
	glCreateFramebuffers(1, &fbo);
	bind();
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::bind(FramebufferTarget target) {
	glBindFramebuffer(static_cast<GLenum>(target), fbo);
}

void Framebuffer::attachTexture(FramebufferAttachment attachment, std::shared_ptr<Texture> texture) {
	glNamedFramebufferTexture(fbo, static_cast<GLenum>(attachment), texture->getHandle(), 0);
}

