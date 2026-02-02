#include "AssetManager.hpp"

#include <ranges>
#include <cctype>
#include <cassert>
#include <iostream>

AssetManager& AssetManager::get() {
	static AssetManager instance;
	return instance;
}

AssetManager::~AssetManager() {
	unload_all();
}

void AssetManager::Load(const std::filesystem::path& path) {
	auto extension = path.extension().string();
	std::ranges::transform(extension, extension.begin(), [](auto c) {
		return static_cast<char>(std::tolower(c));
	});

	auto pathStr = path.string();
	auto assetName = path.stem().string();

#define LOAD_AND_EMPLACE(Map, LoadFunc) \
	auto asset = LoadFunc(pathStr.c_str()); \
	Map[assetName] = asset

#define LOAD_INFO_AND_EMPLACE(Type, Map, LoadFunc) \
	Type info; \
	info.pData = LoadFunc(pathStr.c_str(), &info.count); \
	Map[assetName] = info

	// Texture
	if (extension == ".png" || extension == ".bmp" || extension == ".tga" ||
		extension == ".jpg" || extension == ".jpeg" || extension == ".gif" ||
		extension == ".pic" || extension == ".ppm" || extension == ".psd")
	{
		LOAD_AND_EMPLACE(m_Textures, LoadTexture);
	}
	// Font
	else if (extension == ".ttf" || extension == ".otf" ||
			 extension == ".fnt" || extension == ".bdf")
	{
		LOAD_AND_EMPLACE(m_Fonts, LoadFont);
	}
	// Model + AnimData
	else if (extension == ".obj" || extension == ".iqm" || extension == ".glb" ||
			 extension == ".gltf" || extension == ".vox" || extension == ".m3d")
	{
		LOAD_AND_EMPLACE(m_Models, LoadModel);

		if (extension == ".iqm" || extension == ".m3d" ||
			extension == ".gltf" || extension == ".glb")
		{
			LOAD_INFO_AND_EMPLACE(ModelAnimationInfo, m_ModelAnims, LoadModelAnimations);
		}
	}
	// Materials
	else if (extension == ".mtl")
	{
		LOAD_INFO_AND_EMPLACE(MaterialInfo, m_Materials, LoadMaterials);
	}
	// Sound
	else if (extension == ".wav" || extension == ".ogg" || extension == ".mp3" ||
			 extension == ".qoa" || extension == ".flac")
	{
		LOAD_AND_EMPLACE(m_Sounds, LoadSound);
	}
	else
	{
		std::cerr << "Invalid asset path: " << path << std::endl;
		assert(!"Invalid asset path! See console error!");
	}
}

void AssetManager::LoadMusic(const std::filesystem::path& path) {
	auto extension = path.extension().string();
	std::ranges::transform(extension, extension.begin(), [](auto c) {
		return static_cast<char>(std::tolower(c));
	});

	auto pathStr = path.string();
	auto assetName = path.stem().string();
	
	if (extension == ".wav" || extension == ".ogg" || extension == ".mp3" ||
		extension == ".qoa" || extension == ".flac" || extension == ".xm" ||
		extension == ".mod")
	{
		LOAD_AND_EMPLACE(m_Music, LoadMusicStream);
	}
	else
	{
		std::cerr << "Invalid asset path: " << path << std::endl;
		assert(!"Invalid asset path! See console error!");
	}
}

#undef LOAD_INFO_AND_EMPLACE
#undef LOAD_AND_EMPLACE

void AssetManager::unload_all() {
#define UNLOAD_MAP(Map, UnloadFunc) \
	for (auto& [_, asset] : Map) { \
		UnloadFunc(asset); \
	} \
	Map.clear();
#define UNLOAD_INFO_MAP(Map, UnloadFunc) \
	for (auto& [_, info] : Map) { \
		for (int i = 0; i < info.count; ++i) { \
			UnloadFunc(info.pData[i]); \
		} \
		RL_FREE(info.pData); \
	} \
	Map.clear();

	UNLOAD_MAP(m_Textures, UnloadTexture);
	UNLOAD_MAP(m_Fonts, UnloadFont);
	UNLOAD_MAP(m_Models, UnloadModel);
	UNLOAD_MAP(m_Sounds, UnloadSound);
	UNLOAD_MAP(m_Music, UnloadMusicStream);
	UNLOAD_INFO_MAP(m_ModelAnims, UnloadModelAnimation);
	UNLOAD_INFO_MAP(m_Materials, UnloadMaterial);

#undef UNLOAD_INFO_MAP
#undef UNLOAD_MAP
}
