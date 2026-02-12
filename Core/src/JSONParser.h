#include <fstream>
#include <nlohmann/json.hpp>

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
		ScenesData = nlohmann::json::parse(f)["scenes"];
	}


}