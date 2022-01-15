#pragma once

#include "fwd.h"
#include "utils.hpp"

class AssetManager {

public:

	AssetManager();
	~AssetManager();

	std::shared_ptr<Texture> loadTexture(const std::string path);
	std::shared_ptr<Texture> loadTexture(const std::string path, TextureParams params);
	std::shared_ptr<Mesh> loadMesh(const std::string path);

private:

	std::map<std::string, std::shared_ptr<Texture> > textures;
	std::map<std::string, std::shared_ptr<Mesh> > meshs;
};
