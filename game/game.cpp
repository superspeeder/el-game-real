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

	delete vbo;
	delete vao;
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

	float vdata[6] = { 0,0, 1,1, 0,1 };

	vbo = new VertexBuffer(vdata, 6);
	vao = new VertexArray();
	vao->vertexBuffer(vbo, {2});

	prog = new ShaderProgram({"shaders/main.frag.glsl", "shaders/main.vert.glsl"});
}

void Game::draw() {
	renderer->draw();

	prog->use();
	renderer->drawArrays(vao, PrimitiveMode::Triangles, 3);
}

int main()
{
	Game().run();
	return 0;
}
