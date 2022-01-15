#include "AssetManager.h"

#include "Mesh.h"
#include "Texture.h"

AssetManager::AssetManager() {
}

AssetManager::~AssetManager() {
}

std::shared_ptr<Texture> AssetManager::loadTexture(const std::string path) {
	if (!textures.contains(path)) textures[path] = std::make_shared<Texture>(path);
	return textures[path];
}

std::shared_ptr<Texture> AssetManager::loadTexture(const std::string path, TextureParams params) {
	if (!textures.contains(path)) textures[path] = std::make_shared<Texture>(path, params);
	return textures[path];
}

std::shared_ptr<Mesh> AssetManager::loadMesh(const std::string path) {
	if (!meshs.contains(path)) meshs[path] = std::make_shared<Mesh>(path);
	return meshs[path];
}
