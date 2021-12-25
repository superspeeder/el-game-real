#include "Mesh.h"
#include "Renderer.h"

/*
ex json:
{
	"vertices": [
		0,0,
		1,0,
		1,1,
		0,1
	],
	"indices": [
		0,1,2,3
	],
	"attributes": [
		2
	],
	"primitive": "triangle-fan"
}


primitives:
points,

lines,
line-loop,
line-strip,
lines-adjacency,
line-strip-adjacency,

triangles,
triangles-adjacency,
triangle-strip,
triangle-strip-adjacency,
triangle-fan,

patches

*/

Mesh::Mesh(const rapidjson::Value& val) {
	if (!val.IsObject()) throw std::runtime_error("Cannot read json object for mesh.");
	auto obj = val.GetObject();

	auto verts_i = obj.FindMember("vertices");
	auto indcs_i = obj.FindMember("indices");
	auto attrs_i = obj.FindMember("attributes");


	if (verts_i == obj.end()) throw std::runtime_error("Cannot find a \"vertices\" member in mesh json");
	if (indcs_i == obj.end()) throw std::runtime_error("Cannot find a \"indices\" member in mesh json");
	if (attrs_i == obj.end()) throw std::runtime_error("Cannot find a \"attributes\" member in mesh json");

	auto &verts = verts_i->value;
	auto &attrs = attrs_i->value;
	auto &indcs= indcs_i->value;

	if (!verts.IsArray()) throw std::runtime_error("\"vertices\" member of mesh json is not an array");
	if (!indcs.IsArray()) throw std::runtime_error("\"indices\" member of mesh json is not an array");
	if (!attrs.IsArray()) throw std::runtime_error("\"attributes\" member of mesh json is not an array");

	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	std::vector<uint32_t> attributes;

	auto vert_arr = verts.GetArray();
	auto indc_arr = indcs.GetArray();
	auto attr_arr = attrs.GetArray();

	for (auto& v_ : vert_arr) {
		vertices.push_back(v_.GetFloat());
	}

	for (auto& i_ : indc_arr) {
		indices.push_back(i_.GetUint());
	}

	for (auto& a_ : attr_arr) {
		attributes.push_back(a_.GetUint());
	}

	vbo = std::make_shared<VertexBuffer>(vertices);
	ibo = std::make_shared<IndexBuffer>(indices);
	vao = std::make_shared<VertexArray>();
	vao->elementBuffer(ibo);
	vao->vertexBuffer(vbo, attributes);
}

Mesh::Mesh(const rapidjson::Document& doc) {
	if (!doc.IsObject()) throw std::runtime_error("Cannot read json object for mesh.");
	auto obj = doc.GetObject();

	auto verts_i = obj.FindMember("vertices");
	auto indcs_i = obj.FindMember("indices");
	auto attrs_i = obj.FindMember("attributes");


	if (verts_i == obj.end()) throw std::runtime_error("Cannot find a \"vertices\" member in mesh json");
	if (indcs_i == obj.end()) throw std::runtime_error("Cannot find a \"indices\" member in mesh json");
	if (attrs_i == obj.end()) throw std::runtime_error("Cannot find a \"attributes\" member in mesh json");

	auto& verts = verts_i->value;
	auto& attrs = attrs_i->value;
	auto& indcs = indcs_i->value;

	if (!verts.IsArray()) throw std::runtime_error("\"vertices\" member of mesh json is not an array");
	if (!indcs.IsArray()) throw std::runtime_error("\"indices\" member of mesh json is not an array");
	if (!attrs.IsArray()) throw std::runtime_error("\"attributes\" member of mesh json is not an array");

	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	std::vector<uint32_t> attributes;

	auto vert_arr = verts.GetArray();
	auto indc_arr = indcs.GetArray();
	auto attr_arr = attrs.GetArray();

	for (auto& v_ : vert_arr) {
		vertices.push_back(v_.GetFloat());
	}

	for (auto& i_ : indc_arr) {
		indices.push_back(i_.GetUint());
	}

	for (auto& a_ : attr_arr) {
		attributes.push_back(a_.GetUint());
	}

	vbo = std::make_shared<VertexBuffer>(vertices);
	ibo = std::make_shared<IndexBuffer>(indices);
	vao = std::make_shared<VertexArray>();
	vao->elementBuffer(ibo);
	vao->vertexBuffer(vbo, attributes);
}


Mesh::Mesh(const std::string& path) {
	rapidjson::Document doc;

	std::string text = readFile(path);
	doc.Parse(text.c_str());

	if (!doc.IsObject()) throw std::runtime_error("Cannot read json object for mesh.");
	auto obj = doc.GetObject();

	auto verts_i = obj.FindMember("vertices");
	auto indcs_i = obj.FindMember("indices");
	auto attrs_i = obj.FindMember("attributes");


	if (verts_i == obj.end()) throw std::runtime_error("Cannot find a \"vertices\" member in mesh json");
	if (indcs_i == obj.end()) throw std::runtime_error("Cannot find a \"indices\" member in mesh json");
	if (attrs_i == obj.end()) throw std::runtime_error("Cannot find a \"attributes\" member in mesh json");

	auto& verts = verts_i->value;
	auto& attrs = attrs_i->value;
	auto& indcs = indcs_i->value;

	if (!verts.IsArray()) throw std::runtime_error("\"vertices\" member of mesh json is not an array");
	if (!indcs.IsArray()) throw std::runtime_error("\"indices\" member of mesh json is not an array");
	if (!attrs.IsArray()) throw std::runtime_error("\"attributes\" member of mesh json is not an array");

	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	std::vector<uint32_t> attributes;

	auto vert_arr = verts.GetArray();
	auto indc_arr = indcs.GetArray();
	auto attr_arr = attrs.GetArray();

	for (auto& v_ : vert_arr) {
		vertices.push_back(v_.GetFloat());
	}

	for (auto& i_ : indc_arr) {
		indices.push_back(i_.GetUint());
	}

	for (auto& a_ : attr_arr) {
		attributes.push_back(a_.GetUint());
	}

	vbo = std::make_shared<VertexBuffer>(vertices);
	ibo = std::make_shared<IndexBuffer>(indices);
	vao = std::make_shared<VertexArray>();
	vao->elementBuffer(ibo);
	vao->vertexBuffer(vbo, attributes);
}