#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

enum SceneType {

	BINOMIAL_SCENE

};

NLOHMANN_JSON_SERIALIZE_ENUM(SceneType, {
	{BINOMIAL_SCENE, "BinomialScene"}
});


namespace Core {

	inline static nlohmann::json ScenesData;

	inline static void BuildScenesFromFile() {
		std::ifstream f("assets/scenes.json");

		if (!f.is_open()) {
			std::cerr << "[ERROR] Failed to open scenes metadata" << std::endl;
		}


		ScenesData = nlohmann::json::parse(f)["scenes"];
	}


}