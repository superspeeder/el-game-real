#pragma once

#include "fwd.h"
#include "utils.hpp"
#include "Window.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "SpriteBatch.h"
#include "Camera.h"
#include "event.hpp"
#include "Framebuffer.h"

#include <iostream>


namespace game_constants {
	constexpr float kFreeCameraSpeed = 8.f;
	constexpr glm::uvec2 kRenderResolution{ 1920, 1080 };
}


enum class CameraMode {
	Freecam,
	FollowPlayer
};

class Game {
public:
	Game();
	~Game();

	void init();
	void run();
	void draw();

	void onKey(KeyPressEvent* evt);
	void onFB(WindowFramebufferResizeEvent* evt);

	void switchCameraMode();

	inline const std::shared_ptr<AsyncEventManager> getEventManager() const { return eventManager; };
	inline const std::shared_ptr<OnDemandEventManager> getAppEventManager() const { return appEventManager; };

	void freecamUpdate();

	std::shared_ptr<ShaderProgram> getFontShader();

private:
	std::shared_ptr<Window> window;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<ShaderProgram> prog, fontShader, screenShader;
	std::shared_ptr<Mesh> mesh, screenMesh;
	std::shared_ptr<Texture> texture, gameRenderFBOTex;
	std::shared_ptr<SpriteBatch> spriteBatch;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Framebuffer> gameRenderFramebuffer;

	RendererDraw drawF, screenDrawF;



	glm::vec4 colorA = glm::vec4(1.f, 1.f, 0.f, 1.f);
	glm::vec4 colorB = glm::vec4(1.f, 0.f, 1.f, 1.f);

	CosineWave f{ 0, 1, 2, 1 };
	SimpleFunctionalGradient gradient{colorA, colorB, f};
	std::shared_ptr<OnDemandEventManager> appEventManager;
	std::shared_ptr<AsyncEventManager> eventManager;
	std::mutex fbsize_mtx;
	glm::ivec2 fbsize;

	EventLayer* eventLayer;
	EventLayer* appEventLayer;

	CameraMode cameraMode = CameraMode::Freecam;

	uint64_t frameCounter = 0;
	
};