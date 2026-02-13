#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>	
#include <vector>
#include "Math.h"

constexpr float DEG_TO_RAD = 0.0174533; // pi / 180

enum SceneType {

	BINOMIAL_SCENE

};

NLOHMANN_JSON_SERIALIZE_ENUM(SceneType, {
	{BINOMIAL_SCENE, "BinomialScene"}
});

struct ParsedJSONBodyData {
	
	Core::Engine::ShapeType type;
	float mass;
	float friction;
	Core::Engine::Vec2 initialPosition;
	float initialAngle;

	union {

		Core::Engine::Vec2 dimensions;
		float radius;

	};

	float restitution;
	bool isStatic;

};

struct ParsedJSONSceneData {

	Core::Engine::Vec2 gravity;
	std::vector<ParsedJSONBodyData> bodies;

};

namespace Core::JSONParser {

	static nlohmann::json ScenesData;

	static void BuildScenesFromJSON() {
		std::ifstream f("assets/scenes.json");

		if (!f.is_open()) {
			std::cerr << "[ERROR] Failed to open scenes metadata" << std::endl;
		}


		ScenesData = nlohmann::json::parse(f)["scenes"];
	}

	static ParsedJSONSceneData ParseSceneData(SceneType sceneTypeSerialized) {

		nlohmann::json& scene = ScenesData[sceneTypeSerialized];

		ParsedJSONSceneData sceneData;

		sceneData.gravity = { scene["gravity"]["x"], scene["gravity"]["y"] };

		for (auto& body : scene["bodies"]) {

			ParsedJSONBodyData bodyData{};

			bodyData.mass = body["mass"];
			bodyData.restitution = body["restitution"];
			bodyData.friction = body["friction"];
			bodyData.isStatic = body["isStatic"];
			bodyData.initialPosition = { body["initialPosition"]["x"], body["initialPosition"]["y"] };
			bodyData.initialAngle = body["initialAngle"] * DEG_TO_RAD;

			if (body["type"] == "circle") {
				bodyData.type = Core::Engine::ShapeType::Circle;
				bodyData.radius = body["radius"];

			}
			if (body["type"] == "box") {
				bodyData.type = Core::Engine::ShapeType::OBB;
				bodyData.dimensions = { body["dimensions"]["width"], body["dimensions"]["height"] };
			}

			sceneData.bodies.push_back(bodyData);

		}

		return sceneData;

	}


}