#pragma once

#include "fwd.h"
#include "utils.hpp"

#include "Buffer.h"

class Mesh {
public:
	Mesh(const rapidjson::Value& val);
	Mesh(const rapidjson::Document& doc);
	Mesh(const std::string& path);

	// TODO: Destructor, rendering, getters.

private:

	std::shared_ptr<VertexBuffer> vbo;
	std::shared_ptr<IndexBuffer> ibo;
	std::shared_ptr<VertexArray> vao;

	PrimitiveMode primitive;

};
