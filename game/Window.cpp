#include "Window.h"
#include "game.hpp"

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

void onKey(GLFWwindow* win_, int key, int scancode, int action, int mods) {
	Window* win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(win_));
	win->getGame()->getEventManager()->post(new KeyPressEvent(win, key, action, mods));
}

Game* Window::getGame() const {
	return game;
}

Window::Window(Game* game_, WindowSettings& settings_) : game(game_), settings(settings_) {
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, settings.debugContextEnabled);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	if (settings.monitor == -1) {
		window = glfwCreateWindow(settings.size.x, settings.size.y, settings.title.c_str(), NULL, NULL);
	}
	else {
		int c;
		GLFWmonitor* mon = glfwGetMonitors(&c)[settings.monitor];

		const GLFWvidmode* vm = glfwGetVideoMode(mon);
		glfwWindowHint(GLFW_RED_BITS, vm->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, vm->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, vm->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, vm->refreshRate);

		window = glfwCreateWindow(vm->width, vm->height, settings.title.c_str(), mon, NULL);
	}


	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, onWindowFramebufferResize);

	glfwSetKeyCallback(window, onKey);
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

bool Window::getKey(int k) {
	return glfwGetKey(window, k);
}

