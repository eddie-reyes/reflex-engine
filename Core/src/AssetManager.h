
#include <iostream>
#include <string>
#include <filesystem>

enum class AssetType {

	None = 0,
	BUTTON_TEXTURE,
	BUTTON_TEXTURE_HOVER

};

namespace Core::AssetManager {

	inline Texture2D LoadAssetTexture(AssetType type) {

		std::filesystem::path path;

		switch (type) {
		case AssetType::BUTTON_TEXTURE:

			path = "assets/button.png";
			break;

		case AssetType::BUTTON_TEXTURE_HOVER:

			path = "assets/button_h.png";
			break;
			
		default:
			break;
		}

		if (!std::filesystem::exists(path)) {

			std::cerr << "[ERROR]: Failed to load texture: " << path << std::endl;
			return {};

		}

		std::string resultStr = path.string();

		return LoadTexture(resultStr.c_str());
	}

}