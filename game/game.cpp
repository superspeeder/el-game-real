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

	init();

	glfwSetTime(0);

	while (window->isOpen()) {
		glfwPollEvents();
		
		draw();

		window->swap();
	}
}

void Game::init() {

	renderer = std::make_shared<Renderer>();

	renderer->setBackgroundColor(colors::CYAN);

	std::vector<float> vdata = {0,0, 1,0, 1,1, 0,1};

    vbo = std::make_shared<VertexBuffer>(vdata);
    vbo->bind();
    vao = std::make_shared<VertexArray>();
    vao->vertexBuffer(vbo, {2});

	prog = ShaderProgram::create({ "shaders/main.frag.glsl", "shaders/main.vert.glsl" });

//	drawF.drawData.renderMode = RendererMode::Arrays;
//	drawF.drawData.primitive = PrimitiveMode::TriangleFan;
//	drawF.drawData.vao = vao;
//	drawF.drawData.first = 0;
//	drawF.drawData.count = 4;

	drawF.shader = prog;
}


void Game::draw() {
	drawF.foregroundColor = gradient(glfwGetTime());
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
