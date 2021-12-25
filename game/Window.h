#pragma once

#include "fwd.h"
#include "utils.hpp"

#include <string>

struct WindowSettings {
	glm::ivec2 size;
	std::string title;
	int32_t monitor = -1;
	bool debugContextEnabled = false;

	WindowSettings& setSize(glm::ivec2 size);
	WindowSettings& setSize(int32_t w, int32_t h);

	WindowSettings& setMonitor(int32_t mid);
	WindowSettings& setPrimaryMonitor();


	WindowSettings& setTitle(const std::string& title);

	WindowSettings& debugContext(bool dc = true);
};

class Window {
public:

	Window(WindowSettings& settings);
	~Window();

	bool isOpen() const;
	void swap();

	bool isDebug() const noexcept;

private:
	GLFWwindow* window;
	WindowSettings settings;

};
