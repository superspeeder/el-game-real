#include "game.hpp"

Game::Game() {
	glfwInit();

	WindowSettings settings{};
	settings.size = { 800, 800 };
	settings.debugContext();
	settings.title = "Test Window";

	window = new Window(settings);

	renderer = new Renderer();
}

Game::~Game() {

	delete renderer;
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

	renderer->setBackgroundColor(colors::CYAN);

	glCreateBuffers(1, &testBuf);
	glCreateVertexArrays(1, &testVAO);

	glVertexArrayVertexBuffer(testVAO, 0, testBuf, 0, 2 * sizeof(float));
	glVertexArrayAttribFormat(testVAO, 0, 2, GL_FLOAT, false, 0);
	glVertexArrayAttribBinding(testVAO, 0, 0);
	glEnableVertexArrayAttrib(testVAO, 0);

	float vdata[6] = { 0,0, 1,1, 0,1 };

	glBindBuffer(GL_ARRAY_BUFFER, testBuf);
	glNamedBufferData(testBuf, 6 * sizeof(float), vdata, GL_STATIC_DRAW);
}

void Game::draw() {
	renderer->draw();

	glBindVertexArray(testVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main()
{
	Game().run();
	return 0;
}
