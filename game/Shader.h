#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "utils.hpp"

class Shader {
public:
    Shader(std::string file);
    ~Shader();

    inline uint32_t getHandle() const noexcept { return handle; };  

private:

    uint32_t handle;

};

class ShaderProgram {
public:

    ShaderProgram(std::initializer_list<std::string> files);
    ~ShaderProgram();

    inline uint32_t getHandle() const noexcept { return handle; };  

    void use();

    uint32_t getUniformLocation(std::string& uname);

private:

    uint32_t handle;
};

/*

struct ShaderProgram {
	uint32_t handle;
};

std::string readFile(std::string name) {
	std::ifstream f(name);
	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	return str;
}

constexpr std::string s2lower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

uint32_t sTypeFromName(std::string name) {
	std::string ln = s2lower(name);
	if (ln == "vertex") return GL_VERTEX_SHADER;
	if (ln == "fragment") return GL_FRAGMENT_SHADER;
	if (ln == "compute") return GL_COMPUTE_SHADER;
	if (ln == "geometry") return GL_GEOMETRY_SHADER;
	if (ln == "tess-control") return GL_TESS_CONTROL_SHADER;
	if (ln == "tess-evaluation") return GL_TESS_EVALUATION_SHADER;
	spdlog::error("Unknown shader type {}", name);
	throw std::runtime_error("Unknown shader type " + name);
}

bool isWhiteSpace(char c) {
	if (c == ' ' || c == '\n' || c == '\r' || c == '\t') return true;
	return false;
}

std::string lStripNewlines(std::string text) {
	size_t i = 0;
	char c = text[0];
	while (isWhiteSpace(c)) {
		c = text[++i];
		if (i >= text.size()) return "";
	}

	return text.substr(i);
}

std::tuple<std::string, std::string> splitAt(std::string text, size_t i) {
	if (i == 0) return std::make_tuple("", text.substr(1));
	if (i == std::string::npos) return std::make_tuple(text, "");
	if (i + 1 == text.size()) return std::make_tuple(text.substr(0, i), "");
	if (i + 1 > text.size()) return std::make_tuple(text, "");
	return std::make_tuple(text.substr(0, i), text.substr(i + 1));
}

std::tuple<std::string, std::string> splitOnFirst(std::string text, char c) {
	size_t i = text.find_first_of(c);
	return splitAt(text, i);
}

std::tuple<std::string, std::string> splitFirstLine(std::string text) {
	return splitOnFirst(text, '\n');
}

std::string stripWhitespace(std::string str) {
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

uint32_t createShader(std::string path) {
	std::string srcuf = lStripNewlines(readFile(path));
//expect that the first non-blank line is in format
//	#type <name>


	std::string l, src;
	std::tie(l, src) = splitFirstLine(srcuf);

	if (!l.starts_with("#type ")) { spdlog::error("Failed to read shader file {}. Missing a type header.", path); throw std::runtime_error("Failed to read shader " + path); }

	std::string type = stripWhitespace(l.substr(6));
	uint32_t stype = sTypeFromName(type);

	uint32_t sh = glCreateShader(stype);
	const char* ssrc_czs = src.c_str();
	glShaderSource(sh, 1, &ssrc_czs, nullptr);
	
	glCompileShader(sh);

	int i;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &i);
	if (!i) {
		glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &i);
		char* ilog = new char[i];

		glGetShaderInfoLog(sh, i, &i, ilog);
		spdlog::error("Failed to compile shader {}!", path);
		std::cerr << ilog;

		delete[i] ilog;
		throw std::runtime_error("Failed to compile shader " + path);
	}

	return sh;
}

ShaderProgram* createShaderProgram(std::initializer_list<std::string> files) {
	std::vector<uint32_t> shaders;
	for (std::string p : files) {
		shaders.push_back(createShader(p));
	}
	uint32_t pr = glCreateProgram();
	for (uint32_t s : shaders) {
		glAttachShader(pr, s);
	}

	glLinkProgram(pr);
	int i;
	glGetProgramiv(pr, GL_LINK_STATUS, &i);
	if (!i) {
		glGetProgramiv(pr, GL_INFO_LOG_LENGTH, &i);
		char* ilog = new char[i];

		glGetProgramInfoLog(pr, i, &i, ilog);
		spdlog::error("Failed to link program");
		std::cerr << ilog;

		delete[i] ilog;
		throw std::runtime_error("Failed to link program");
	}
	
	for (uint32_t s : shaders) {
		glDeleteShader(s);
	}

	return new ShaderProgram{pr};
}
*/