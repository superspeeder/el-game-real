#pragma once

#include "fwd.h"
#include "utils.hpp"

namespace framebuffer_constants {
	constexpr uint32_t kDefaultFramebuffer = 0;
}


class Framebuffer {
public:

	static void bindDefault(FramebufferTarget target = FramebufferTarget::Both);
	
	Framebuffer();
	~Framebuffer();

	void bind(FramebufferTarget target = FramebufferTarget::Both);

	void attachTexture(FramebufferAttachment attachment, std::shared_ptr<Texture> texture);

private:

	std::unordered_map<FramebufferAttachment, std::shared_ptr<Texture> > attachments;

	uint32_t fbo;
};
