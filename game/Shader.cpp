#include "Shader.h"
#include <iostream>
#include <vector>

Shader::Shader(std::string file) {
	std::string srcuf = lStripNewlines(readFile(file));

	std::string l, src;
	std::tie(l, src) = splitFirstLine(srcuf);

	if (!l.starts_with("#type ")) { throw std::runtime_error("Failed to read handleader " + file); }

	std::string type = stripWhitespace(l.substr(6));
	uint32_t stype = sTypeFromName(type);

	handle = glCreateShader(stype);
	const char* ssrc_czs = src.c_str();
	glShaderSource(handle, 1, &ssrc_czs, nullptr);
	
	glCompileShader(handle);

	int i;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &i);
	if (!i) {
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &i);
		char* ilog = new char[i];

		glGetShaderInfoLog(handle, i, &i, ilog);
		std::cerr << ilog;

		delete[i] ilog;
		throw std::runtime_error("Failed to compile shader " + file);
	}
}

Shader::~Shader() {
    glDeleteShader(handle);
}

ShaderProgram::ShaderProgram(std::initializer_list<std::string> files) {
    std::vector<std::shared_ptr<Shader> > shaders;
	for (std::string p : files) {
		shaders.push_back(std::make_shared<Shader>(p));
	}
	handle = glCreateProgram();
	for (const auto& s : shaders) {
		glAttachShader(handle, s->getHandle());
	}

	glLinkProgram(handle);
	int i;
	glGetProgramiv(handle, GL_LINK_STATUS, &i);
	if (!i) {
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &i);
		char* ilog = new char[i];

		glGetProgramInfoLog(handle, i, &i, ilog);
		std::cerr << ilog;

		delete[i] ilog;
		throw std::runtime_error("Failed to link program");
	}
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(handle);
}

void ShaderProgram::use() {
    glUseProgram(handle);
}

uint32_t ShaderProgram::getUniformLocation(std::string& uname) {
    return glGetUniformLocation(handle, uname.c_str());
}

