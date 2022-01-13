#pragma once

#include "fwd.h"
#include <rapidjson/document.h>

#include <glm/gtc/constants.hpp>

#include <iostream>
#include <string>
#include <tuple>
#include <algorithm>
#include <exception>
#include <fstream>
#include <sstream>
#include <functional>
#include <condition_variable>
#include <vector>
#include <concepts>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include <thread>


template<typename T>
struct dirtyable {
    T value;
    bool dirty = true;

    inline dirtyable(T val) : value{val} {};
    inline void operator=(const T& val) { value = val; dirty = true; };
    inline bool isDirty() const noexcept { return dirty; };
    inline void clean() { dirty = false; };
    inline void markDirty() { dirty = true; };
    inline const T* operator->() { return &value; };
};

class DirtyableType {
public:

    inline void markDirty() { dirty = true; };
    inline bool isDirty() const noexcept { return dirty; };

    inline void clean() { dirty = false; }; 

private:

    bool dirty;
};

template<typename T>
class dirty_this {
private:
    T value;
    DirtyableType* target;

public:

    inline dirty_this(T val, DirtyableType* target_) : value{val}, target{target_} {
    };

    inline T* operator->() const noexcept { return &value; };

    inline void operator=(const T& val) { value = val; target->markDirty(); };
};

inline std::string readFile(std::string name) {
	std::ifstream f(name);
	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	return str;
}

inline std::string s2lower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

inline uint32_t sTypeFromName(std::string name) {
	std::string ln = s2lower(name);
	if (ln == "vertex") return GL_VERTEX_SHADER;
	if (ln == "fragment") return GL_FRAGMENT_SHADER;
	if (ln == "compute") return GL_COMPUTE_SHADER;
	if (ln == "geometry") return GL_GEOMETRY_SHADER;
	if (ln == "tess-control") return GL_TESS_CONTROL_SHADER;
	if (ln == "tess-evaluation") return GL_TESS_EVALUATION_SHADER;
	throw std::runtime_error("Unknown shader type " + name);
}

inline bool isWhiteSpace(char c) {
	if (c == ' ' || c == '\n' || c == '\r' || c == '\t') return true;
	return false;
}

inline std::string lStripNewlines(std::string text) {
	size_t i = 0;
	char c = text[0];
	while (isWhiteSpace(c)) {
		c = text[++i];
		if (i >= text.size()) return "";
	}

	return text.substr(i);
}

inline std::tuple<std::string, std::string> splitAt(std::string text, size_t i) {
	if (i == 0) return std::make_tuple("", text.substr(1));
	if (i == std::string::npos) return std::make_tuple(text, "");
	if (i + 1 == text.size()) return std::make_tuple(text.substr(0, i), "");
	if (i + 1 > text.size()) return std::make_tuple(text, "");
	return std::make_tuple(text.substr(0, i), text.substr(i + 1));
}

inline std::tuple<std::string, std::string> splitOnFirst(std::string text, char c) {
	size_t i = text.find_first_of(c);
	return splitAt(text, i);
}

inline std::tuple<std::string, std::string> splitFirstLine(std::string text) {
	return splitOnFirst(text, '\n');
}

inline std::string stripWhitespace(std::string str) {
	size_t start = 0;
	char c = str[start];
	while (isWhiteSpace(c)) {
		c = str[++start];
		if (start >= str.size()) return "";
	}
	std::string rem = str.substr(start);
	size_t end = rem.size() - 1;
	c = rem[end];
	while (isWhiteSpace(c)) {
		c = rem[--end];
		if (end == 0 || end >= rem.size()) return "";
	}

	return rem.substr(0, end + 1);
}

struct CosineWave {
	float lower, upper, period, shift = 0.f;

	inline float operator()(float t) {
		return ((upper - lower) * glm::cos((2.f * glm::pi<float>() / period) * (t + shift)) / 2.0f + (upper + lower) / 2.0f);
	};
};

inline void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
};

inline void setupDebug() {
	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_TRUE);
	}
};

struct SimpleFunctionalGradient {
	glm::vec4 colorA, colorB;

	std::function<float(float)> positionCalculator;

	inline glm::vec4 operator()(float x) {
		return glm::mix(colorA, colorB, positionCalculator(x));
	};
};

namespace funct {
	template<typename T, typename TT>
	concept consumer = requires(T v, TT b) { v(b); };
}

template<typename T>
concept only_incrementally = requires(T v) { v++; v--; };

template<only_incrementally T, T* target_value>
struct raii_counter {
	inline raii_counter() {
		(*target_value)++;
	};
	inline ~raii_counter() {
		(*target_value)--;
	};
};

#define RAII_COUNTER(varn) raii_counter<decltype(varn), &varn>

template<only_incrementally T, funct::consumer<T> oninc, funct::consumer<T> ondec>
struct watched_counter {
	T value = 0;
	oninc f_oninc{};
	ondec f_ondec{};

	watched_counter() {
	};

	watched_counter(T v) : value(v) {
	};

	inline T operator++() {
		T old = value++;
		f_oninc(value);
		return old;
	};

	inline watched_counter& operator--() {
		T old = value--;
		f_ondec(value);
		return this;
	};

	inline watched_counter& operator++(int) {
		value++;
		f_oninc(value);
		return *this;
	};

	inline watched_counter& operator--(int) {
		value--;
		f_ondec(value);
		return *this;
	};

	inline bool operator==(const T& other) {
		return value == other;
	};

	inline bool operator==(const watched_counter<T, oninc, ondec>& other) {
		return value == other.value;
	};
};

template <typename T>
struct ptr_less {
	inline bool operator()(const T* a, const T* b) const {
		return std::less<T>{}(*a, *b);
	};
};

template<size_t count>
class thread_pool {
public:
	template<typename... Args>
	inline thread_pool(std::function<void(std::stop_token)> f) {
		for (size_t i = 0; i < count; i++) {
			threads.emplace_back(f);
		}
	};

	inline void stop() {
		for (std::jthread& t : threads) {
			t.get_stop_source().request_stop();
		}
		for (std::jthread& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	};

	inline void stop(std::condition_variable& var) {
		for (std::jthread& t : threads) {
			t.get_stop_source().request_stop();
		}

		var.notify_all();

		for (std::jthread& t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	};

private:
	std::vector<std::jthread> threads;
};


namespace constmath {
	template <typename T>
	constexpr T ipow(T num, unsigned int pow) {
		return (pow >= sizeof(unsigned int) * 8) ? 0 :
			pow == 0 ? 1 : num * ipow(num, pow - 1);
	}
}

namespace memoryconvert {
	constexpr uint32_t b1024p(uint32_t n) { return constmath::ipow(1024U, n); };

	constexpr uint32_t kb2b(uint32_t n) { return n * b1024p(1); };
	constexpr uint32_t mb2b(uint32_t n) { return n * b1024p(2); };
	constexpr uint32_t gb2b(uint32_t n) { return n * b1024p(3); };

	constexpr float b2kb(uint32_t n) { return (float)n / (float)b1024p(1); };
	constexpr float b2mb(uint32_t n) { return (float)n / (float)b1024p(2); };
	constexpr float b2gb(uint32_t n) { return (float)n / (float)b1024p(3); };
}
