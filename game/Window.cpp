#include "Window.h"

WindowSettings& WindowSettings::setSize(glm::ivec2 size) {
	this->size = size;
	return *this;
}

WindowSettings& WindowSettings::setSize(int32_t w, int32_t h) {
	size.x = w;
	size.y = h;
	return *this;
}


WindowSettings& WindowSettings::setMonitor(int32_t mid) {
	monitor = mid;
	return *this;
}

WindowSettings& WindowSettings::setPrimaryMonitor() {
	monitor = 0;
	return *this;
}

WindowSettings& WindowSettings::setTitle(const std::string& title) {
	this->title = title;
	return *this;
}


WindowSettings& WindowSettings::debugContext(bool dc) {
	debugContextEnabled = dc;
	return *this;
}

void updateViewport(glm::ivec2 newVp) {
	glViewport(0, 0, newVp.x, newVp.y);
}

void onWindowFramebufferResize(GLFWwindow* win, int x, int y) {
	updateViewport({ x, y });
}


Window::Window(WindowSettings& settings_) : settings{ settings_ } {
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.debugContextEnabled);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(settings.size.x, settings.size.y, settings.title.c_str(), NULL, NULL);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(window, onWindowFramebufferResize);
}

bool Window::isOpen() const {
	return !glfwWindowShouldClose(window);
}

void Window::swap() {
	glfwSwapBuffers(window);
}

bool Window::isDebug() const noexcept {
	return settings.debugContextEnabled;
}

Window::~Window() {
	glfwDestroyWindow(window);
}