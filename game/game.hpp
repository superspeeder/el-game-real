#pragma once

#include "fwd.h"
#include "utils.hpp"
#include "Window.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "SpriteBatch.h"

#include <iostream>

class Game {
public:
	Game();
	~Game();

	void init();
	void run();
	void draw();

private:
	std::shared_ptr<Window> window;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<ShaderProgram> prog;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<SpriteBatch> spriteBatch;
	
	RendererDraw drawF;



	glm::vec4 colorA = glm::vec4(1.f, 1.f, 0.f, 1.f);
	glm::vec4 colorB = glm::vec4(1.f, 0.f, 1.f, 1.f);

	CosineWave f{ 0, 1, 2, 1 };
	SimpleFunctionalGradient gradient{colorA, colorB, f};
};