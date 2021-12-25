#pragma once

#include "fwd.h"
#include "utils.hpp"
#include "Window.h"
#include "Renderer.h"
#include "Shader.h"

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
	std::shared_ptr<VertexBuffer> vbo;
	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<ShaderProgram> prog;
	RendererDraw drawF;


	uint32_t testBuf;
	uint32_t testVAO;


	glm::vec4 colorA = glm::vec4(1.f, 1.f, 0.f, 1.f);
	glm::vec4 colorB = glm::vec4(1.f, 0.f, 1.f, 1.f);

	CosineWave f{ 0, 1, 2, 1 };
	SimpleFunctionalGradient gradient{colorA, colorB, f};
};