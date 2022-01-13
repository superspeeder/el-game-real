#pragma once

#include "fwd.h"
#include "utils.hpp"

#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace camera_constants {
	constexpr float kZMin = 000.00f;
	constexpr float kZMax = 100.00f;
}

class Camera {


public:

	Camera(glm::vec4 bounds, float zMin = camera_constants::kZMin, float zMax = camera_constants::kZMax);

	void setPosition(glm::vec2 pos);

	void setBounds(glm::vec4 bounds, float zMin = camera_constants::kZMin, float zMax = camera_constants::kZMax);

	glm::vec2 getPos();


	glm::mat4 getViewProjection();

	void move(glm::vec2 d);

private:

	bool dirty = true;

	glm::mat4 vp;
	glm::mat4 view;
	glm::mat4 projection;

	glm::vec2 pos = { 0.f, 0.f };
};
