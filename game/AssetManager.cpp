#include "AssetManager.h"

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

AssetManager::AssetManager(std::shared_ptr<NameMapper> name_mapper) : nameMapper(name_mapper) {
}

AssetManager::~AssetManager() {
}

std::shared_ptr<Texture> AssetManager::loadTexture(const std::string& name) {
	if (!textures.contains(name)) textures[name] = std::make_shared<Texture>(pathFromLoadingName(name));
	return textures[name];
}

std::shared_ptr<Texture> AssetManager::loadTexture(const std::string& name, TextureParams params) {
	if (!textures.contains(name)) textures[name] = std::make_shared<Texture>(pathFromLoadingName(name), params);
	return textures[name];
}

std::shared_ptr<Mesh> AssetManager::loadMesh(const std::string& name) {
	if (!meshs.contains(name)) meshs[name] = std::make_shared<Mesh>(pathFromLoadingName(name));
	return meshs[name];
}

std::shared_ptr<ShaderProgram> AssetManager::loadShader(const std::string& name) {
	if (!shaders.contains(name)) shaders[name] = std::make_shared<ShaderProgram>(pathFromLoadingName(name));
	return shaders[name];
}

std::string AssetManager::pathFromLoadingName(const std::string& name) {
	if (nameMapper && nameMapper->hasMappingForName(name)) {
		return nameMapper->mapToPath(name).string();
	}

	return name;
}

NameMapper::NameMapper() { }

NameMapper::NameMapper(std::string json) {
	rapidjson::Document doc;

	std::string text = readFile(json);
	doc.Parse(text.c_str());

	if (!doc.IsObject()) {
		spdlog::error("Name Mapping JSON must be an object.");
		throw std::runtime_error("Name Mapping JSON must be an object.");
	}
	
	int error_cnt = 0;
	int mapping_cnt = 0;

	auto basepath = std::filesystem::path(json);


	spdlog::info("Beginning name mapping load from \"{}\"", json);
	for (const auto& kv : doc.GetObject()) {
		if (!kv.name.IsString()) {
			spdlog::error("Name Mapping JSON keys must be strings.");
			error_cnt++;
			continue;
		}

		if (!kv.value.IsString()) {
			spdlog::error("Name Mapping JSON values must be strings. (name: \"{}\")", kv.name.GetString());
			error_cnt++;
			continue;
		}

		std::filesystem::path path = kv.value.GetString();
		if (path.is_relative()) {
			auto np = basepath / path;
			if (!std::filesystem::exists(np)) {
				np = std::filesystem::current_path() / path;
				if (!std::filesystem::exists(np)) {
					spdlog::error("Failed to find file for path \"{}\" in mapping name \"\"", path.string(), kv.name.GetString());
					error_cnt++;
					continue;
				}
			}
			path = np;
		}

		if (addMapping(kv.name.GetString(), path)) mapping_cnt++;
	}

	spdlog::info("Finished loading {} mappings from \"{}\". Errors: {}", mapping_cnt, json, error_cnt);
}

bool NameMapper::addMapping(std::string name, std::filesystem::path path, bool replace) {
	if (replace) {
		if (hasMappingForName(name)) {
			map.by<mapping_name>().erase(name);
		}

		if (hasMappingForPath(name)) {
			map.by<mapping_path>().erase(path);
		}

		map.insert(map_val(name, path));
	}
	else {
		if (hasMappingForName(name)) {
			spdlog::warn("Failed to add mapping \"{}\" -> \"{}\" because there is already a mapping for this name", name, path.string());
			return false;
		}

		if (hasMappingForPath(path)) {
			spdlog::warn("Failed to add mapping \"{}\" -> \"{}\" because there is already a mapping for the target path", name, path.string());
			return false;
		}

		map.insert(map_val(name, path));
	}

	spdlog::debug("Added asset name mapping \"{}\" -> \"{}\"", name, path.string());
	return true;
}

const std::filesystem::path& NameMapper::mapToPath(const std::string& name) {
	return map.by<mapping_name>().find(name).dereference().get<mapping_path>();
}

const std::string& NameMapper::mapToName(const std::filesystem::path& path) {
	return map.by<mapping_path>().find(path).dereference().get<mapping_name>();
}

bool NameMapper::hasMappingForName(const std::string& name) {
	auto mapbyname = map.by<mapping_name>();
	return mapbyname.find(name) != mapbyname.end();
}

bool NameMapper::hasMappingForPath(const std::filesystem::path& path) {
	auto mapbypath = map.by<mapping_path>();
	return mapbypath.find(path) != mapbypath.end();
}


