#pragma once

#include "fwd.h"
#include "utils.hpp"

#include <filesystem>
#include <memory>

#include <boost/bimap.hpp>

class NameMapper {
public:
	NameMapper();
	NameMapper(std::string json);

	bool addMapping(std::string name, std::filesystem::path path, bool replace = false);
	
	const std::filesystem::path& mapToPath(const std::string& name);
	const std::string& mapToName(const std::filesystem::path& path);

	bool hasMappingForName(const std::string& name);
	bool hasMappingForPath(const std::filesystem::path& path);

private:
	struct mapping_name {};
	struct mapping_path {};

	using map_type = boost::bimap<boost::bimaps::tagged<std::string, mapping_name>, boost::bimaps::tagged<std::filesystem::path, mapping_path> >;
	using map_val = map_type::value_type;
	using map_by_name = map_type::map_by<mapping_name>;
	using map_by_path = map_type::map_by<mapping_path>;

	map_type map;

};

class AssetManager {

public:

	AssetManager(std::shared_ptr<NameMapper> name_mapper = nullptr);
	~AssetManager();

	std::shared_ptr<Texture> loadTexture(const std::string& name);
	std::shared_ptr<Texture> loadTexture(const std::string& name, TextureParams params);
	std::shared_ptr<Mesh> loadMesh(const std::string& name);
	std::shared_ptr<ShaderProgram> loadShader(const std::string& name);


private:

	/**
	 * @brief Convert a name to path if possible
	 * 
	 * Convert the passed in string into a path. Search Order:
	 *	1. If a name mapper is set, try to use the name mapper to map the path
	 *  2. Otherwise, there is no known mapping, so just return the passed in name
	 * 
	 * @param name the name to convert
	 * @return the path (if a conversion is known, else just the passed in string)
	*/
	std::string pathFromLoadingName(const std::string& name);

	std::map<std::string, std::shared_ptr<Texture> > textures;
	std::map<std::string, std::shared_ptr<Mesh> > meshs;
	std::map<std::string, std::shared_ptr<ShaderProgram> > shaders;

	std::shared_ptr<NameMapper> nameMapper;
};
