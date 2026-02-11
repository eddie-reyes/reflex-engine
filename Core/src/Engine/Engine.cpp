#include "Engine.h"
#include "Collision.h"
#include "Impulse.h"


namespace Core::Engine {

    static Vec2 gravity = { 0, 9.8f };

	Engine::Engine()
	{
		

		m_Scene.push_back(std::make_unique<Circle>(10, 0.5, 0.1, 50));
		m_Scene.push_back(std::make_unique<Circle>(100, 0.5, 0.1, 50));
		//Vec2 boundingBox = { (float)GetScreenWidth(), 100 };
		//m_Scene.push_back(std::make_unique<Core::Engine::Box>(STATIC_BODY, 1, 0.1, boundingBox));
		//m_Scene[0]->Position = { (float)GetScreenWidth() / 2 - 25.0f , (float)GetScreenHeight() / 3 };
		//m_Scene[1]->Position = { (float)GetScreenWidth() / 2 , (float)GetScreenHeight() / 2 };
		//m_Scene[2]->Position = { (float)GetScreenWidth() / 2 , (float)GetScreenHeight() - 50 };
	}

	void Engine::Tick(float dt)
	{

        // 1) apply forces + integrate velocities
        for (auto& b : m_Scene) {
            if (b->IsStatic()) continue; //ignore static bodies

			b->Velocity += gravity * dt * SIM_SPEED;

        }

        // 2) build contacts
        std::vector<Core::Engine::Manifold> contacts;
        contacts.reserve(m_Scene.size());

        const int n = (int)m_Scene.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) { //narrowphase
                Core::Engine::Body& A = *m_Scene[i];
                Core::Engine::Body& B = *m_Scene[j];
                if (A.IsStatic() && B.IsStatic()) continue;

                Core::Engine::Manifold m;
                if (BuildManifold(A, B, m)) {
                    m.ia = i; m.ib = j;
                    contacts.push_back(m);
                }
            }
        }

        // 3) iterative impulse solve
        for (int it = 0; it < SOLVER_ITER; ++it) {
            for (auto& c : contacts) {
                ApplyImpulse(*m_Scene[c.ia], *m_Scene[c.ib], c);
            }
        }

        // 4) integrate positions
        for (auto& b : m_Scene) {
            if (b->IsStatic()) continue;
            b->Position += b->Velocity * dt * SIM_SPEED;
        }

        // 5) positional correction
        for (auto& c : contacts) {
            PositionalCorrection(*m_Scene[c.ia], *m_Scene[c.ib], c);
        }


	}

}

