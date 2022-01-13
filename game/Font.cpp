#include "Font.h"

FT_Library ftLibrary;

void initFreetype() {
	if (FT_Init_FreeType(&ftLibrary)) {
		spdlog::error("Failed to initialize FreeType");
	}
}

FontFamily::FontFamily(const std::string& ttfFile) {
	if (FT_New_Face(ftLibrary, ttfFile.c_str(), 0, &face)) {
		spdlog::error("Failed to load font '{}'", ttfFile);
	}
}

FontFamily::~FontFamily() {
    FT_Done_Face(face);
}

void cleanupFreetype() {
    FT_Done_FreeType(ftLibrary);
}

Font::Font(std::shared_ptr<FontFamily> family_, int size_) : family(family_), size(size_) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    FT_Set_Pixel_Sizes(family->getFace(), 0, size);

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(family->getFace(), c, FT_LOAD_RENDER)) {
            spdlog::error("Failed to load glyph for '{}'", (char)c);
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            family->getFace()->glyph->bitmap.width,
            family->getFace()->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            family->getFace()->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        textures.push_back(texture);

        Glyph glyph = {
            texture,
            glm::ivec2(family->getFace()->glyph->bitmap.width, family->getFace()->glyph->bitmap.rows),
            glm::ivec2(family->getFace()->glyph->bitmap_left, family->getFace()->glyph->bitmap_top),
            family->getFace()->glyph->advance.x
        };
        
        glyphs[(char)c] = glyph;
    }
}

Font::~Font() {
    glDeleteTextures(textures.size(), textures.data());
}
