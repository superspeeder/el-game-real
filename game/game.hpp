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

#include <iostream>


namespace game_constants {
	constexpr float kFreeCameraSpeed = 8.f;
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

private:
	std::shared_ptr<Window> window;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<ShaderProgram> prog;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Texture> texture;
	std::shared_ptr<SpriteBatch> spriteBatch;
	std::shared_ptr<Camera> camera;
	
	RendererDraw drawF;



	glm::vec4 colorA = glm::vec4(1.f, 1.f, 0.f, 1.f);
	glm::vec4 colorB = glm::vec4(1.f, 0.f, 1.f, 1.f);

	CosineWave f{ 0, 1, 2, 1 };
	SimpleFunctionalGradient gradient{colorA, colorB, f};
	std::shared_ptr<OnDemandEventManager> appEventManager;
	std::shared_ptr<AsyncEventManager> eventManager;

	EventLayer* eventLayer;
	EventLayer* appEventLayer;

	CameraMode cameraMode = CameraMode::Freecam;
	
};