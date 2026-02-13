#include "Engine.h"
#include "Collision.h"
#include "Impulse.h"

namespace Core::Engine {

	Engine::Engine()
	{
        Reset();
        JSONParser::BuildScenesFromJSON();
	}

    void Engine::BuildScene(SceneType sceneTypeSerialized) {

		ParsedJSONSceneData sceneData = JSONParser::ParseSceneData(sceneTypeSerialized);

        for (const auto& bodyData : sceneData.bodies) {
        
            switch (bodyData.type) {

            case ShapeType::Circle: 
                m_Scene.push_back(std::make_unique<Circle>(bodyData.mass, bodyData.restitution, bodyData.friction, bodyData.isStatic, bodyData.initialPosition, bodyData.initialAngle, bodyData.radius));
                break;
            
            case ShapeType::OBB:
                m_Scene.push_back(std::make_unique<Box>(bodyData.mass, bodyData.restitution, bodyData.friction, bodyData.isStatic, bodyData.initialPosition, bodyData.initialAngle, bodyData.dimensions));
                break;
            default:
                break;
            }
        }
       
    }

	void Engine::Tick(float dt)
	{
        if (m_isPaused) return;

        float dt_s = (dt * SIM_SPEED) / SUBSTEPS;

        //substep each tick for stability
        for (int s = 0; s < SUBSTEPS; ++s) {
           
            // 1) apply forces + integrate velocities
            for (auto& b : m_Scene) {
                if (b->IsStatic()) continue; //ignore static bodies

                b->Velocity += m_Gravity * dt_s;

                b->AngularVelociy *= ANGULAR_DAMPING; //apply damping 

            }

            // 2) build contacts
            std::vector<Manifold> contacts;
            contacts.reserve(m_Scene.size());

            // naive approach O(n ^ 2) : checks all collision pairs
			// ideally should implement something like broadphase collision detection in future
            for (int i = 0; i < m_Scene.size(); ++i) {
                for (int j = i + 1; j < m_Scene.size(); ++j) { 
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
            for (int it = 0; it < IMPULSE_SOLVER_ITER; ++it) {
                for (auto& c : contacts) {
                    ApplyImpulse(*m_Scene[c.ia], *m_Scene[c.ib], c);
                }
            }

            // 4) integrate positions and angles
            for (auto& b : m_Scene) {
                if (b->IsStatic()) continue;
                b->Position += b->Velocity * dt_s;
                b->Angle += b->AngularVelociy * dt_s;

            }

            // 5) positional correction
            for (auto& c : contacts) {
                PositionalCorrection(*m_Scene[c.ia], *m_Scene[c.ib], c);
            }
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

    void Engine::MapSceneCoordsToWindow(float screenWidth, float screenHeight) {

        for (auto& b : m_Scene) {

            float projectedX = (screenWidth / 100) * b->Position.x;
            float projectedY = (screenHeight / 100) * b->Position.y;

			b->Position = { projectedX, projectedY };
            
        }
    }

}

