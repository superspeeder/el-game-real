#include "Texture.h"
#include <stb_image.h>
#include <stb_image_write.h>

Texture::Texture(glm::uvec2 textureSize_, const TextureParams& params) : textureSize(textureSize_) {
	glCreateTextures(GL_TEXTURE_2D, 1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);

	glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, textureSize.x, textureSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);


}

uint32_t fmtForChNum(int nrChannels) {
	switch (nrChannels) {
	case 1: return GL_R;
	case 2: return GL_RG;
	case 3: return GL_RGB;
	case 4: return GL_RGBA;
	}
	return GL_RGBA;
}

Texture::Texture(const std::string& textureFile, const TextureParams& params) {

	stbi_set_flip_vertically_on_load(true);
	
	glCreateTextures(GL_TEXTURE_2D, 1, &handle);

	glBindTexture(GL_TEXTURE_2D, handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, params.internalFormat, width, height, 0, fmtForChNum(nrChannels), GL_UNSIGNED_BYTE, data);
		if (params.genMipMaps) {
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	else {
		spdlog::error("Failed to load texture '{}'", textureFile);
	}

	stbi_image_free(data);

	textureSize = { width, height };
}

Texture::~Texture() {
	glDeleteTextures(1, &handle);
}

const glm::uvec2& Texture::getTextureSize() const noexcept {
	return textureSize;
}

void Texture::bind(uint32_t unit) {
	glBindTextureUnit(unit, handle);
}



