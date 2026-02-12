#include "Engine.h"
#include "Collision.h"
#include "Impulse.h"
#include <iostream>

namespace Core::Engine {

	Engine::Engine()
	{
        Reset();
        BuildScenesFromFile();
	}

    void Engine::BuildScene(SceneType sceneTypeSerialized) {

        nlohmann::json scene = ScenesData[sceneTypeSerialized];
        
        m_Gravity = { scene["gravity"]["x"], scene["gravity"]["y"] };

        for (auto& body : scene["bodies"]) {

            float mass = body["mass"];
            float restitution = body["restitution"];
            float friction = body["friction"];
			bool isStatic = body["isStatic"];
			Vec2 initialPosition = { body["initialPosition"]["x"], body["initialPosition"]["y"] };

            if (body["type"] == "circle") {
				float radius = body["radius"];
                m_Scene.push_back(std::make_unique<Circle>(mass, restitution, friction, isStatic, initialPosition, radius));
            }
            if (body["type"] == "box") {
                Vec2 dimensions = { body["dimensions"]["width"], body["dimensions"]["height"] };
                m_Scene.push_back(std::make_unique<Box>(mass, restitution, friction, isStatic, initialPosition, dimensions));
            }

        }
       
    }

	void Engine::Tick(float dt)
	{
        if (m_isPaused) return;

        // 1) apply forces + integrate velocities
        for (auto& b : m_Scene) {
            if (b->IsStatic()) continue; //ignore static bodies

			b->Velocity += m_Gravity * dt * SIM_SPEED;

        }

        // 2) build contacts
        std::vector<Manifold> contacts;
        contacts.reserve(m_Scene.size());

        const int n = (int)m_Scene.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) { //narrowphase
                Body& A = *m_Scene[i];
                Body& B = *m_Scene[j];
                if (A.IsStatic() && B.IsStatic()) continue;

                Manifold m;
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

    void Engine::TogglePause() {
		m_isPaused = !m_isPaused;
    }

    void Engine::Reset()
    {

		m_Scene.clear();
		m_isPaused = true;

    }

}

