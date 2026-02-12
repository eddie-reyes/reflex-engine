
#include <vector>
#include <memory>
#include "Body.h"
#include "JSONParser.h"

#define SIM_SPEED 5.0f;

constexpr auto SOLVER_ITER = 10;

namespace Core::Engine {

	class Engine {

	public:
		Engine();

		void Tick(float tickRate);

		auto& GetSceneBodies() { return m_Scene; };

		void BuildScene(SceneType sceneSerialized);

		void Pause() { m_isPaused = true; };

		void UnPause() { m_isPaused = false; };

		void Reset();

	private:

		std::vector<std::unique_ptr<Body>> m_Scene; //collection of bodies

		bool m_isPaused;

		Vec2 m_Gravity = {0.0f, 9.81f}; 

	};

}
