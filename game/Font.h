#pragma once

#include "fwd.h"
#include "utils.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

extern FT_Library ftLibrary;
void initFreetype();
void cleanupFreetype();

struct Glyph {
	uint32_t texID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	uint32_t advance;
};

class FontFamily {
public:

	FontFamily(const std::string& ttfFile);

	~FontFamily();

	inline FT_Face getFace() const noexcept { return face; };

private:

	FT_Face face;
};

class Font {
public:

	Font(std::shared_ptr<FontFamily> family, int size);
	~Font();

private:
	std::shared_ptr<FontFamily> family;
	std::map<char, Glyph> glyphs;
	std::vector<uint32_t> textures;
	int size;
};

class RenderableText {
public:

	RenderableText(std::shared_ptr<Font> font, glm::vec3 color, std::strnig text);

private:

};