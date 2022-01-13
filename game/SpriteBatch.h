#pragma once
#include "fwd.h"
#include "utils.hpp"


namespace spritebatch_constants {
	constexpr uint32_t kMaxBufferSize = memoryconvert::mb2b(24);
	constexpr uint32_t kVertexSize = 5;
	constexpr uint32_t kMaxVertices = kMaxBufferSize / (kVertexSize * sizeof(float));
	constexpr uint32_t kMaxQuads = kMaxVertices / 4;
	constexpr uint32_t kIndicesPerQuad = 6;
	constexpr PrimitiveMode kPrimitiveMode = PrimitiveMode::Triangles;
}

class SpriteBatch {
public:

	static void init();
	static void cleanup();

	SpriteBatch();
	~SpriteBatch();

	const std::shared_ptr<Mesh>& getMesh();

	void conservativeFlush();

	void begin();
	void end();

	// texCoords is {u1,v1,u2,v2}
	void batch(glm::vec2 pos, glm::vec2 size, glm::vec4 texCoords, float z = 1.0f);
	// quad is {x,y,w,h}
	void batch(glm::vec4 quad, glm::vec4 texCoords, float z = 1.0f);

private:

	std::shared_ptr<VertexBuffer> vbo;
	std::shared_ptr<VertexArray> vao;
	std::shared_ptr<Mesh> mesh;

	bool open = false;

	uint32_t currentQuad = 0;
};
