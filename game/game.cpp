#include "game.hpp"

Game::Game() {
	WindowSettings settings{};
	settings.size = { 800, 800 };
	settings.debugContext();
	settings.title = "Test Window";

	window = std::make_shared<Window>(settings);
}

Game::~Game() {
}

void Game::run() {
	if (window->isDebug()) {
		setupDebug();
	}

	spdlog::info("Hello!");

	SpriteBatch::init();

	init();

	glfwSetTime(0);

	while (window->isOpen()) {
		glfwPollEvents();
		
		draw();

		window->swap();
	}

	SpriteBatch::cleanup();
}

void Game::init() {

	renderer = std::make_shared<Renderer>();

	renderer->setBackgroundColor(colors::CYAN);

	prog = ShaderProgram::create({ "shaders/main.frag.glsl", "shaders/main.vert.glsl" });

	spriteBatch = std::make_shared<SpriteBatch>();

//	mesh = std::make_shared<Mesh>("meshs/quad.json");
	mesh = spriteBatch->getMesh();

	texture = std::make_shared<Texture>("textures/texture.png");

	drawF.mesh = mesh;
	drawF.shader = prog;
	drawF.textures[0] = texture;
}


void Game::draw() {
//	drawF.foregroundColor = gradient(glfwGetTime());

	spriteBatch->begin();
	spriteBatch->batch({ -1,0,0.5,0.5 }, { 0,0,128,128 });
	spriteBatch->batch({ 0,0,0.5,0.5 }, { 0,0,128,128 });
	spriteBatch->batch({ 0,-1,0.5,0.5 }, { 0,0,128,128 });
	spriteBatch->end();

	renderer->queue(drawF);


	renderer->draw();
}



int main() {
	// initialize
	glfwInit();

	Game().run();

	// deinitialize
	glfwTerminate();
	return 0;
}
