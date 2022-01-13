#include "SpriteBatch.h"

#include "Buffer.h"
#include "Texture.h"
#include "Mesh.h"
#include <array>

using namespace spritebatch_constants;

std::shared_ptr<IndexBuffer> ibo;

void SpriteBatch::init() {
	ibo = std::make_shared<IndexBuffer>(kMaxQuads * kIndicesPerQuad);
	uint32_t *data = new uint32_t[kMaxQuads * kIndicesPerQuad];
	for (uint32_t q = 0; q < kMaxQuads; q++) {
		uint32_t start = q * kIndicesPerQuad;
		uint32_t firstV = q * 4;

		data[start] = firstV;
		data[start + 1] = firstV + 1;
		data[start + 2] = firstV + 2;

		data[start + 3] = firstV;
		data[start + 4] = firstV + 2;
		data[start + 5] = firstV + 3;
	}

	ibo->set(data, kMaxQuads * kIndicesPerQuad);
	delete[kMaxQuads * kIndicesPerQuad] data;
}

void SpriteBatch::cleanup() {
}

SpriteBatch::SpriteBatch() {

	vbo = std::make_shared<VertexBuffer>(kMaxVertices * kVertexSize, BufferMode::DynamicDraw);

	vao = std::make_shared<VertexArray>();
	vao->elementBuffer(ibo->getHandle());
	vao->vertexBuffer(vbo, { 2, 2 });

	mesh = std::make_shared<Mesh>(vbo, ibo, vao, kPrimitiveMode);
}

SpriteBatch::~SpriteBatch() {

}


const std::shared_ptr<Mesh>& SpriteBatch::getMesh() {
	conservativeFlush();
	return mesh;
}

void SpriteBatch::conservativeFlush() {
	vbo->conservativeFlush();
}

void SpriteBatch::end() {
	conservativeFlush();
	open = false;
	mesh->setIndicesToRender(currentQuad * kIndicesPerQuad);
}

void SpriteBatch::begin() {
	open = true;
	currentQuad = 0; // effective reset;
}

// texCoords is {u1,v1,u2,v2}
void SpriteBatch::batch(glm::vec2 pos, glm::vec2 size, glm::vec4 texCoords) {
	if (currentQuad >= kMaxQuads) {
		spdlog::warn("SpriteBatch Full, cannot batch more quads. Consider optimizing your renderer dumbass.");
		return;
	}

	size_t curIndex = currentQuad * kVertexSize * 4;

	std::array<float, kVertexSize * 4> qvs{
		// v1
		pos.x, pos.y,
		texCoords.x, texCoords.y,

		// v2
		pos.x + size.x, pos.y,
		texCoords.z, texCoords.y,

		// v3
		pos.x + size.x, pos.y + size.y,
		texCoords.z, texCoords.w,

		// v4
		pos.x, pos.y + size.y,
		texCoords.x, texCoords.z
	};

	vbo->setRegion(curIndex, qvs);
	currentQuad++;
}

// quad is {x,y,w,h}
void SpriteBatch::batch(glm::vec4 quad, glm::vec4 texCoords) {
	batch({ quad.x, quad.y }, { quad.z, quad.w }, texCoords);
}

