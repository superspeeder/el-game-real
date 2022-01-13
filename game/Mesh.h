#pragma once

#include "fwd.h"
#include "utils.hpp"

#include "Buffer.h"

#include <optional>

class Mesh {
public:
	Mesh(const rapidjson::Value& val);
	Mesh(const rapidjson::Document& doc);
	Mesh(const std::string& path);
	Mesh(const std::shared_ptr<VertexBuffer>& vbo, const std::shared_ptr<IndexBuffer>& ibo, const std::shared_ptr<VertexArray>& vao, PrimitiveMode primitiveMode);

	// TODO: Destructor, rendering, getters.

	inline const std::shared_ptr<VertexBuffer>& getVertexBuffer() const noexcept { return vbo; };
	inline const std::shared_ptr<IndexBuffer>& getIndexBuffer() const noexcept { return ibo; };
	inline const std::shared_ptr<VertexArray>& getVertexArray() const noexcept { return vao; };

	inline const PrimitiveMode getPrimitiveMode() const noexcept { return primitive; };

	uint32_t getIndicesToRender();

	inline void setIndicesToRender(uint32_t count) { indexcount = count; };

private:

	std::shared_ptr<VertexBuffer> vbo;
	std::shared_ptr<IndexBuffer> ibo;
	std::shared_ptr<VertexArray> vao;

	PrimitiveMode primitive;

	std::optional<uint32_t> indexcount;

};
