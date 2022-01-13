#pragma once

#include "fwd.h"
#include "utils.hpp"

#include <string>
#include "event.hpp"



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

	Window(Game* game, WindowSettings& settings);
	~Window();

	bool isOpen() const;
	void swap();

	bool isDebug() const noexcept;

	Game* getGame() const;
	
	bool getKey(int k);

private:
	GLFWwindow* window;
	WindowSettings settings;
	Game* game;

};

class KeyPressEvent : public BaseEvent {
public:

	inline KeyPressEvent(Window* window_, int key_, int action_, int mods_) : window(window_), key(key_), action(action_), mods(mods_) {
	};

	inline int getKey() const noexcept { return key; };
	inline int getAction() const noexcept { return action; };
	inline int getMods() const noexcept { return mods; };
	inline Window* getWindow() const noexcept { return window; };

private:
	int key, action, mods;
	Window* window;


};


class WindowFramebufferResizeEvent : public BaseEvent {
public:

	inline WindowFramebufferResizeEvent(Window* window_, glm::ivec2 size_) : window(window_), size(size) {
	};

	inline glm::ivec2 getSize() const noexcept { return size; };
	inline Window* getWindow() const noexcept { return window; };

private:
	glm::ivec2 size;
	Window* window;
};