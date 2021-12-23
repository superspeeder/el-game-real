#pragma once

#include <iostream>
#include "Window.h"
#include "Renderer.h"

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

	uint32_t testBuf;
	uint32_t testVAO;
};