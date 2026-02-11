
#include <vector>
#include <memory>
#include "Body.h"

#define SIM_SPEED 5.0f;
constexpr auto SOLVER_ITER = 10;

namespace Core::Engine {


	class Engine {

	public:
		Engine();

		void Tick(float tickRate);

		std::vector<std::unique_ptr<Body>>& GetScene() { return m_Scene; }

	private:

		std::vector<std::unique_ptr<Body>> m_Scene; //the scene is a collection of bodies

		float m_TickRate;


	};

}
