#pragma once

#include <iostream>
#include "Window.h"

class Game {
public:
	Game();
	~Game();

	void init();
	void run();
	void draw();

private:
	Window* window;
};