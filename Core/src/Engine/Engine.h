
#include <vector>
#include <memory>
#include "Body.h"
#include "JSONParser.h"

constexpr auto SIM_SPEED = 10.0f; //speed factor
constexpr auto IMPULSE_SOLVER_ITER = 10; //impulse solver
constexpr auto SUBSTEPS = 5; //substeps per tick of simulation
constexpr auto ANGULAR_DAMPING = 0.98f; //prevents bodies from spinning indefinitely
constexpr auto SCENE_BOUNDS = 200.0; //max x,y coord

namespace Core::Engine {

	class Engine {

	public:
		Engine();

		void Tick(float tickRate);

		auto& GetSceneBodies() { return m_Scene; };

		void BuildScene(SceneType sceneTypeSerialized);

		void ClearScene();

		void ResetScene();

		SceneType GetCurrentScene() const { return m_CurrentScene;  } ;

		float GetBounds() const { return m_Bounds; };

		void MapSceneCoordsToWindow(float in_end, float out_end);

		Vec2 ProjectPositionToWindow(Vec2 pos);

		bool IsPaused() { return m_isPaused; };

		void TogglePause();

		bool AddCircle(float mass, float radius, float restituion, float friction, bool isStatic, Vec2 initalPosition, float initialAngle, Color color, int identifier);

		bool AddBox(float mass, Vec2 dimensions, float restituion, float friction, bool isStatic, Vec2 initalPosition, float initialAngle, Color color, int identifier);

	private:

		std::vector<std::unique_ptr<Body>> m_Scene; //collection of bodies

		bool m_isPaused;

		SceneType m_CurrentScene;

		float m_Bounds;

		Vec2 m_Gravity;

	};

}
