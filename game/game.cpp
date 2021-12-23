#include "game.hpp"

Game::Game() {
	glfwInit();

	WindowSettings settings{};
	settings.size = { 800, 800 };
	settings.debugContext();
	settings.title = "Test Window";

	window = new Window(settings);
}

Game::~Game() {

	delete window;
	glfwTerminate();
}

void Game::run() {
	init();

	while (window->isOpen()) {
		glfwPollEvents();
		
		draw();

		window->swap();
	}
}

void Game::init() {
	glClearColor(1, 0, 0, 1);
}

void Game::draw() {
	glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{
	Game().run();
	return 0;
}
