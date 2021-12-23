#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <tuple>
#include <algorithm>
#include <exception>
#include <fstream>
#include <sstream>


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

