#pragma once

#include "fwd.h"
#include "utils.hpp"

struct TextureParams {
	uint32_t minFilter = GL_LINEAR;
	uint32_t magFilter = GL_NEAREST;
	uint32_t wrapS = GL_CLAMP_TO_EDGE;
	uint32_t wrapT = GL_CLAMP_TO_EDGE;
	uint32_t internalFormat = GL_RGBA;
	bool genMipMaps = false;
};

constexpr TextureParams kDefaultTexParams{};

class Texture {
public:

	Texture(glm::uvec2 textureSize_, const TextureParams& params = kDefaultTexParams);
	Texture(const std::string& textureFile, const TextureParams& params = kDefaultTexParams);
	~Texture();

	const glm::uvec2& getTextureSize() const noexcept;

	void bind(uint32_t unit);

private:
	uint32_t handle;
	glm::uvec2 textureSize;
};
