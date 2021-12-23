#pragma once

#include <iostream>
#include "Window.h"
#include "Renderer.h"
#include "Shader.h"

class Game {
public:
	Game();
	~Game();

	void init();
	void run();
	void draw();

private:
	Window* window;
	Renderer* renderer;
	VertexBuffer* vbo;
	VertexArray* vao;
	ShaderProgram* prog;

	uint32_t testBuf;
	uint32_t testVAO;
};