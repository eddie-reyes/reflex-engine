
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <memory>


enum class AssetType {

	None = 0,
	BUTTON_TEXTURE,
	BUTTON_TEXTURE_HOVERED,
	BINOMIAL_SCENE_IMG

};

struct TextureAsset {

	AssetType type;
	Texture2D texture;

};

namespace Core::AssetManager {

	inline const std::vector<std::pair<AssetType, std::string>> texturePaths = { {AssetType::BUTTON_TEXTURE, "assets/button.png"}, {AssetType::BUTTON_TEXTURE_HOVERED, "assets/button_h.png"}, {AssetType::BINOMIAL_SCENE_IMG, "assets/button_h.png"} };

	inline std::vector<TextureAsset> textureAssets;

	inline void LoadTextures() {

		for (const auto& [type, filepath] : texturePaths) {

			std::filesystem::path path = filepath;

			if (!std::filesystem::exists(path)) {

				std::cerr << "[ERROR] Failed to load texture: " << path << std::endl;

			}

			std::string resultStr = path.string();

			TextureAsset asset = { type, LoadTexture(resultStr.c_str()) };
			textureAssets.push_back(asset);
		}
		 
	}

	inline Texture2D LoadAssetTexture(AssetType type) {

		for (const auto& asset: textureAssets) {

			if (asset.type == type) {

				return asset.texture;
			}
		}
		
	}

}