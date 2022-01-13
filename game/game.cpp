#include "game.hpp"

using namespace game_constants;

void Game::onKey(KeyPressEvent* evt) {
	if (evt->getAction() == GLFW_PRESS) {
		if (evt->getKey() == GLFW_KEY_SPACE) {
			switchCameraMode();
		}
	}
}

void Game::onFB(WindowFramebufferResizeEvent* evt) {
	camera->setBounds({ -evt->getSize().x / 2, -evt->getSize().y / 2, evt->getSize().x / 2, evt->getSize().y / 2 });
}

void Game::switchCameraMode() {
	if (cameraMode == CameraMode::Freecam) cameraMode = CameraMode::FollowPlayer;
	else cameraMode = CameraMode::Freecam;
}

Game::Game() {
	appEventManager = std::make_shared<OnDemandEventManager>();
	eventManager = std::make_shared<AsyncEventManager>();
	eventLayer = eventManager->createLayer(1);
	appEventLayer = appEventManager->createLayer(1);

	eventLayer->addListener<KeyPressEvent>(&Game::onKey, this);


	WindowSettings settings{};
	settings.size = { 1920, 1080 };
	settings.debugContext();
	settings.title = "Test Window";

	window = std::make_shared<Window>(this, settings);
}

Game::~Game() {
}

void Game::run() {
	if (window->isDebug()) {
		setupDebug();
	}

	spdlog::info("Hello!");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SpriteBatch::init();


	init();

	glfwSetTime(0);

	while (window->isOpen()) {
		glfwPollEvents();
		
		draw();

		window->swap();
	}

	SpriteBatch::cleanup();

	eventManager->end_after_done();
}

void Game::init() {

	renderer = std::make_shared<Renderer>();

	renderer->setBackgroundColor(colors::CYAN);

	prog = ShaderProgram::create({ "shaders/main.frag.glsl", "shaders/main.vert.glsl" });

	spriteBatch = std::make_shared<SpriteBatch>();

//	mesh = std::make_shared<Mesh>("meshs/quad.json");
	mesh = spriteBatch->getMesh();

	texture = std::make_shared<Texture>("textures/texture.png");

	camera = std::make_shared<Camera>(glm::vec4{ -960, -540, 960, 540 });


	renderer->setCamera(camera);

	drawF.mesh = mesh;
	drawF.shader = prog;
	drawF.textures[0] = texture;

	spdlog::info("tsize: {},{}", texture->getTextureSize().x, texture->getTextureSize().y);
}


void Game::freecamUpdate() {
	if (window->getKey(GLFW_KEY_A)) {
		camera->move({ -kFreeCameraSpeed, 0 });
	}
	if (window->getKey(GLFW_KEY_D)) {
		camera->move({ kFreeCameraSpeed, 0 });
	}
	if (window->getKey(GLFW_KEY_W)) {
		camera->move({ 0, kFreeCameraSpeed });
	}
	if (window->getKey(GLFW_KEY_S)) {
		camera->move({ 0, -kFreeCameraSpeed });
	}
}

constexpr float ts = 16.f;

void Game::draw() {
//	drawF.foregroundColor = gradient(glfwGetTime());

	switch (cameraMode) {
	case CameraMode::Freecam: freecamUpdate(); break;
	}

	spriteBatch->begin();

	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			spriteBatch->batch({ x * ts, y * ts, ts, ts }, { 16,16,16,16 });
		}
	}


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
