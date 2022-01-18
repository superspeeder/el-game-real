#include "game.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

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
	fbsize_mtx.lock();
	fbsize = evt->getSize();
	fbsize_mtx.unlock();
}

void Game::switchCameraMode() {
	if (cameraMode == CameraMode::Freecam) cameraMode = CameraMode::FollowPlayer;
	else cameraMode = CameraMode::Freecam;
}

Game::Game() {
	auto stdoutsink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	auto filesink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/game.log", memoryconvert::mb2b(16), 30, false);

	std::shared_ptr<spdlog::logger> logger = std::make_shared<spdlog::logger>("main", spdlog::sinks_init_list{ stdoutsink, filesink });
#if GAME_DEBUG
	logger->set_level(spdlog::level::debug);
#else // #if GAME_DEBUG
	logger->set_level(spdlog::level::info);
#endif // #if GAME_DEBUG

	logger->flush_on(spdlog::level::err);
	spdlog::set_default_logger(logger);
	

	spdlog::info("===============================================================================");
	spdlog::info("================================ Game Starting ================================");
	spdlog::info("===============================================================================");




	appEventManager = std::make_shared<OnDemandEventManager>();
	eventManager = std::make_shared<AsyncEventManager>();
	eventLayer = eventManager->createLayer(1);
	appEventLayer = appEventManager->createLayer(1);

	eventLayer->addListener<KeyPressEvent>(&Game::onKey, this);


	WindowSettings settings{};
	settings.size = { 1920, 1080 };
	settings.debugContext();
	settings.title = "Test Window";
	settings.setPrimaryMonitor();

	window = std::make_shared<Window>(this, settings);

	fbsize_mtx.lock();
	fbsize = settings.size;
	fbsize_mtx.unlock();
}

Game::~Game() {
	spdlog::shutdown();
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

		frameCounter++;
	}
	double t = glfwGetTime();

	spdlog::info("Ran for {} seconds, {} frames. Average frames per second: {}", t, frameCounter, (double)frameCounter / t);

	SpriteBatch::cleanup();

	eventManager->end_after_done();
}

void Game::init() {

	gameRenderFBOTex = std::make_shared<Texture>(kRenderResolution, TextureParams{ GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_RGBA, false, 0 });
	gameRenderFramebuffer = std::make_shared<Framebuffer>();
	gameRenderFramebuffer->attachTexture(FramebufferAttachment::Color0, gameRenderFBOTex);

	renderer = std::make_shared<Renderer>();

	renderer->setBackgroundColor(colors::CYAN);

//	prog = ShaderProgram::create({ "shaders/main.frag.glsl", "shaders/main.vert.glsl" });
//	fontShader = ShaderProgram::create({ "shaders/font.frag.glsl", "shaders/font.vert.glsl" });

	prog = std::make_shared<ShaderProgram>("shaders/main.shader.json");
//	fontShader = std::make_shared<ShaderProgram>("shaders/font.shader.json");
	screenShader = std::make_shared<ShaderProgram>("shaders/screen.shader.json");

	spriteBatch = std::make_shared<SpriteBatch>();

//	mesh = std::make_shared<Mesh>("meshs/quad.json");
	mesh = spriteBatch->getMesh();
	screenMesh = std::make_shared<Mesh>("meshs/screen.json");

	texture = std::make_shared<Texture>("textures/texture.png");

	camera = std::make_shared<Camera>(glm::vec4{ -960, -540, 960, 540 });


	renderer->setCamera(camera);

	drawF.mesh = mesh;
	drawF.shader = prog;
	drawF.textures[0] = texture;

	screenDrawF.mesh = screenMesh;
	screenDrawF.shader = screenShader;
	screenDrawF.textures[0] = gameRenderFBOTex;

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

std::shared_ptr<ShaderProgram> Game::getFontShader() {
	return fontShader;
}

constexpr float ts = 16.f;

void Game::draw() {
//	drawF.foregroundColor = gradient(glfwGetTime());

	int n = window->getKey(GLFW_KEY_LEFT_CONTROL) ? 55 : 53;
	switch (cameraMode) {
	case CameraMode::Freecam: 
		freecamUpdate(); 
		break;
	}


	spriteBatch->begin();
	int i = 0;
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			spriteBatch->batch({ x * ts, y * ts, ts, ts }, { (i % 3) * 16, ((int)(i / 3) % 3) * 16, 16, 16 });
			i++;
		}
	}
	spriteBatch->end();

	renderer->queue(drawF);

	gameRenderFramebuffer->bind();
	glViewport(0, 0, gameRenderFBOTex->getTextureSize().x, gameRenderFBOTex->getTextureSize().y); 

	renderer->setBackgroundColor(colorA);
	renderer->draw();
	
	Framebuffer::bindDefault();
	fbsize_mtx.lock();
	glViewport(0, 0, fbsize.x, fbsize.y);
	fbsize_mtx.unlock();


	renderer->queue(screenDrawF);
	renderer->setBackgroundColor(colorB);
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
