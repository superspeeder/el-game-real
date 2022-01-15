#include "Shader.h"
#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

Shader::Shader(std::string file) {
	std::string srcuf = lStripNewlines(readFile(file));

	std::string l, src;
	std::tie(l, src) = splitFirstLine(srcuf);

	if (!l.starts_with("#type ")) {
		spdlog::critical("Failed to read shader file '{}'", file);
		throw std::runtime_error("Failed to read shader " + file);
	}

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
		spdlog::critical(ilog);

		delete[i] ilog;
		spdlog::critical("Failed to compile shader file '{}'", file);
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
		spdlog::critical(ilog);

		delete[i] ilog;
		spdlog::critical("Failed to link program");
		throw std::runtime_error("Failed to link program");
	}
}


ShaderProgram::ShaderProgram(std::string json) {
	rapidjson::Document doc;

	std::string text = readFile(json);
	doc.Parse(text.c_str());

	if (!doc.IsArray()) throw std::runtime_error("Failed to load shader collection '" + json + "'. JSON must be an array.");
	auto arr = doc.GetArray();

	std::vector<std::shared_ptr<Shader> > shaders;
	for (const auto& v : arr) {
		if (v.IsString()) {
			std::string str = v.GetString();
			std::filesystem::path p = str;
			if (p.is_absolute()) {
				shaders.push_back(std::make_shared<Shader>(p.string()));
			}
			else {
				p = std::filesystem::current_path(); 
				p /= std::filesystem::path(json).parent_path();
				p /= str;
				shaders.push_back(std::make_shared<Shader>(p.string()));
			}
		}
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
		spdlog::critical(ilog);

		delete[i] ilog;
		spdlog::critical("Failed to link program");
		throw std::runtime_error("Failed to link program");
	}
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(handle);
}

void ShaderProgram::use() {
    glUseProgram(handle);
}

uint32_t ShaderProgram::getUniformLocation(const std::string& uname) {
	if (uniforms.contains(uname)) return uniforms[uname];
	glGetUniformLocation(handle, uname.c_str());
}

void ShaderProgram::setInt(const std::string& name, int32_t x) {
	glProgramUniform1i(handle, getUniformLocation(name), x);
}

void ShaderProgram::setUInt(const std::string& name, uint32_t x) {
	glProgramUniform1ui(handle, getUniformLocation(name), x);
}

void ShaderProgram::setFloat(const std::string& name, float x) {
	glProgramUniform1f(handle, getUniformLocation(name), x);
}

void ShaderProgram::setDouble(const std::string& name, double x) {
	glProgramUniform1d(handle, getUniformLocation(name), x);

}

void ShaderProgram::setBool(const std::string& name, bool x) {
	glProgramUniform1i(handle, getUniformLocation(name), x);
}


void ShaderProgram::setInt2(const std::string& name, int32_t x, int32_t y) {
	glProgramUniform2i(handle, getUniformLocation(name), x, y);
}

void ShaderProgram::setUInt2(const std::string& name, uint32_t x, uint32_t y) {
	glProgramUniform2ui(handle, getUniformLocation(name), x, y);
}

void ShaderProgram::setFloat2(const std::string& name, float x, float y) {
	glProgramUniform2f(handle, getUniformLocation(name), x, y);
}

void ShaderProgram::setDouble2(const std::string& name, double x, double y) {
	glProgramUniform2d(handle, getUniformLocation(name), x, y);
}

void ShaderProgram::setBool2(const std::string& name, bool x, bool y) {
	glProgramUniform2i(handle, getUniformLocation(name), x, y);
}

void ShaderProgram::setInt2(const std::string& name, glm::ivec2 x) {
	glProgramUniform2iv(handle, getUniformLocation(name), 1, (const GLint*) &x);
}

void ShaderProgram::setUInt2(const std::string& name, glm::uvec2 x) {
	glProgramUniform2uiv(handle, getUniformLocation(name), 1, (const GLuint*)&x);
}

void ShaderProgram::setFloat2(const std::string& name, glm::fvec2 x) {
	glProgramUniform2fv(handle, getUniformLocation(name), 1, (const GLfloat*)&x);
}

void ShaderProgram::setDouble2(const std::string& name, glm::dvec2 x) {
	glProgramUniform2dv(handle, getUniformLocation(name), 1, (const GLdouble*)&x);
}

void ShaderProgram::setBool2(const std::string& name, glm::bvec2 x) {
	glProgramUniform2iv(handle, getUniformLocation(name), 1, (const GLint*)&x);
}


void ShaderProgram::setInt3(const std::string& name, int32_t x, int32_t y, int32_t z) {
	glProgramUniform3i(handle, getUniformLocation(name), x, y, z);
}

void ShaderProgram::setUInt3(const std::string& name, uint32_t x, uint32_t y, uint32_t z) {
	glProgramUniform3ui(handle, getUniformLocation(name), x, y, z);
}

void ShaderProgram::setFloat3(const std::string& name, float x, float y, float z) {
	glProgramUniform3f(handle, getUniformLocation(name), x, y, z);
}

void ShaderProgram::setDouble3(const std::string& name, double x, double y, double z) {
	glProgramUniform3d(handle, getUniformLocation(name), x, y, z);
}

void ShaderProgram::setBool3(const std::string& name, bool x, bool y, bool z) {
	glProgramUniform3i(handle, getUniformLocation(name), x, y, z);
}


void ShaderProgram::setInt3(const std::string& name, glm::ivec3 x) {
	glProgramUniform3iv(handle, getUniformLocation(name), 1, (const GLint*)&x);
}

void ShaderProgram::setUInt3(const std::string& name, glm::uvec3 x) {
	glProgramUniform3uiv(handle, getUniformLocation(name), 1, (const GLuint*)&x);
}

void ShaderProgram::setFloat3(const std::string& name, glm::fvec3 x) {
	glProgramUniform3fv(handle, getUniformLocation(name), 1, (const GLfloat*)&x);

}

void ShaderProgram::setDouble3(const std::string& name, glm::dvec3 x) {
	glProgramUniform3dv(handle, getUniformLocation(name), 1, (const GLdouble*)&x);
}

void ShaderProgram::setBool3(const std::string& name, glm::bvec3 x) {
	glProgramUniform3iv(handle, getUniformLocation(name), 1, (const GLint*)&x);
}


void ShaderProgram::setInt4(const std::string& name, int32_t x, int32_t y, int32_t z, int32_t w) {
	glProgramUniform4i(handle, getUniformLocation(name), x, y, z, w);
}

void ShaderProgram::setUInt4(const std::string& name, uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
	glProgramUniform4ui(handle, getUniformLocation(name), x, y, z, w);
}

void ShaderProgram::setFloat4(const std::string& name, float x, float y, float z, float w) {
	glProgramUniform4f(handle, getUniformLocation(name), x, y, z, w);
}

void ShaderProgram::setDouble4(const std::string& name, double x, double y, double z, double w) {
	glProgramUniform4d(handle, getUniformLocation(name), x, y, z, w);
}

void ShaderProgram::setBool4(const std::string& name, bool x, bool y, bool z, bool w) {
	glProgramUniform4i(handle, getUniformLocation(name), x, y, z, w);
}


void ShaderProgram::setInt4(const std::string& name, glm::ivec4 x) {
	glProgramUniform4iv(handle, getUniformLocation(name), 1, (const GLint*)&x);
}

void ShaderProgram::setUInt4(const std::string& name, glm::uvec4 x) {
	glProgramUniform4uiv(handle, getUniformLocation(name), 1, (const GLuint*)&x);
}

void ShaderProgram::setFloat4(const std::string& name, glm::fvec4 x) {
	glProgramUniform4fv(handle, getUniformLocation(name), 1, (const GLfloat*)&x);

}

void ShaderProgram::setDouble4(const std::string& name, glm::dvec4 x) {
	glProgramUniform4dv(handle, getUniformLocation(name), 1, (const GLdouble*)&x);
}

void ShaderProgram::setBool4(const std::string& name, glm::bvec4 x) {
	glProgramUniform4iv(handle, getUniformLocation(name), 1, (const GLint*)&x);
}

void ShaderProgram::setMatrix2(const std::string& name, glm::mat2x2 m) {
	glProgramUniformMatrix2fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}

void ShaderProgram::setMatrix3(const std::string& name, glm::mat3x3 m) {
	glProgramUniformMatrix3fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}

void ShaderProgram::setMatrix4(const std::string& name, glm::mat4x4 m) {
	glProgramUniformMatrix4fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}


void ShaderProgram::setMatrix2x3(const std::string& name, glm::mat2x3 m) {
	glProgramUniformMatrix2x3fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}

void ShaderProgram::setMatrix3x2(const std::string& name, glm::mat3x2 m) {
	glProgramUniformMatrix3x2fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}

void ShaderProgram::setMatrix2x4(const std::string& name, glm::mat2x4 m) {
	glProgramUniformMatrix2x4fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}

void ShaderProgram::setMatrix4x2(const std::string& name, glm::mat4x2 m) {
	glProgramUniformMatrix4x2fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}

void ShaderProgram::setMatrix4x3(const std::string& name, glm::mat4x3 m) {
	glProgramUniformMatrix4x3fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}

void ShaderProgram::setMatrix3x4(const std::string& name, glm::mat3x4 m) {
	glProgramUniformMatrix3x4fv(handle, getUniformLocation(name), 1, false, glm::value_ptr(m));
}

