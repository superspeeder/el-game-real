#include "Camera.h"

Camera::Camera(glm::vec4 bounds, float zMin, float zMax) {
	projection = glm::ortho(bounds.x, bounds.z, bounds.y, bounds.w, zMin, zMax);
	view = glm::identity<glm::mat4>();
}

void Camera::setPosition(glm::vec2 pos) {
	this->pos = pos;
	dirty = true;
}

glm::vec2 Camera::getPos() {
	return pos;
}

glm::mat4 Camera::getViewProjection() {
	if (dirty) {
		view = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f));
		vp = projection * view;
		dirty = false;
	}

	return vp;
}
