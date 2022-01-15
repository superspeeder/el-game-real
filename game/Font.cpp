#include "Font.h"
#include "Framebuffer.h"
#include "Texture.h"
#include "game.hpp"

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

RenderableText::RenderableText(Game* game_, std::shared_ptr<Font> font, glm::vec3 color, std::string text) : game(game_) {
    struct CharRData {
        float xpos, ypos, w, h;
        uint32_t tex;
    };

    std::vector<CharRData> rds(text.size());

    renderedText = std::make_shared<Framebuffer>();

    int32_t x1 = 0;
    int32_t x2 = INT32_MIN;
    int32_t y1 = INT32_MAX;
    int32_t y2 = INT32_MIN;


    int xs = 0;
    for (char c : text) {
        const Glyph& g = (*font)[c];
        float xpos = xs + g.bearing.x;
        float ypos = g.size.y - g.bearing.y;

        float w = g.size.x;
        float h = g.size.y;

        if (x1 > xpos) x1 = xpos;
        if (x2 < xpos + w) x2 = xpos + w;

        if (y1 > ypos) y1 = ypos;
        if (y2 < ypos + h) y2 = ypos + h;

        xs += g.advance >> 6;

        rds.push_back({ xpos, ypos, w, h, g.texID });
    }

    width = x2 - x1;
    height = y2 - y1;
    baseline = -y1;

    renderedTextTex = std::make_shared<Texture>(glm::uvec2{ width, height });
    renderedText->attachTexture(FramebufferAttachment::Color0, renderedTextTex);

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_STREAM_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::shared_ptr<ShaderProgram> sp = game->getFontShader();
    
    renderedText->bind();
    sp->use();
    sp->setFloat3("uTextColor", color);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    std::array<float, 6 * 4> vertices;

    for (const auto& rd : rds) {
        glBindTexture(GL_TEXTURE_2D, rd.tex);
        vertices = {
            rd.xpos,        rd.ypos + rd.h,   0.0f, 0.0f,
            rd.xpos,        rd.ypos,          0.0f, 1.0f,
            rd.xpos + rd.w, rd.ypos,          1.0f, 1.0f,

            rd.xpos,        rd.ypos + rd.h,   0.0f, 0.0f,
            rd.xpos + rd.w, rd.ypos,          1.0f, 1.0f,
            rd.xpos + rd.w, rd.ypos + rd.h,   1.0f, 0.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    Framebuffer::bindDefault();
}

RenderableText::~RenderableText() {

}
