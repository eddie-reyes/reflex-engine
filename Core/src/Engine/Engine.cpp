#include "Engine.h"
#include "Collision.h"
#include "Impulse.h"

namespace Core::Engine {

	Engine::Engine()
	{
        ClearScene();
        JSONParser::BuildScenesFromJSON();
	}

    void Engine::BuildScene(SceneType sceneTypeSerialized) {

		ParsedJSONSceneData sceneData = JSONParser::ParseSceneData(sceneTypeSerialized);

        m_Gravity = sceneData.gravity;

        for (const auto& bodyData : sceneData.bodies) {
        
            switch (bodyData.type) {

            case ShapeType::Circle: 
                m_Scene.push_back(std::make_unique<Circle>(bodyData.mass, bodyData.restitution, bodyData.friction, bodyData.isStatic, bodyData.initialPosition, bodyData.initialAngle, bodyData.radius, bodyData.fillColor));
                break;
            
            case ShapeType::OBB:
                m_Scene.push_back(std::make_unique<Box>(bodyData.mass, bodyData.restitution, bodyData.friction, bodyData.isStatic, bodyData.initialPosition, bodyData.initialAngle, bodyData.dimensions, bodyData.fillColor));
                break;
            default:
                break;
            }
        }
        
		m_CurrentScene = sceneTypeSerialized;
       
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

    void Engine::ClearScene()
    {

		m_Scene.clear();
		m_isPaused = false;

    }

    void Engine::ResetScene() {

        ClearScene();
        JSONParser::BuildScenesFromJSON();
		BuildScene(m_CurrentScene);
        MapSceneCoordsToWindow(SCENE_BOUNDS, m_Bounds);

    }

    void Engine::MapSceneCoordsToWindow(float in_end, float out_end) {

        for (auto& b : m_Scene) {

            float projectedX =  (out_end * b->Position.x) / (in_end);
            float projectedY =  (out_end * b->Position.y) / (in_end);

            if (b->Shape.Type == ShapeType::Circle) {
				b->Shape.Radius *= (out_end / in_end); //scale radius by bounds
			}

            if (b->Shape.Type == ShapeType::OBB) {
                b->Shape.Half *= (out_end / in_end); //scale dimensions by bounds
            }

			b->Position = { projectedX, projectedY };

            m_Bounds = out_end;
            
        }
    }

    Vec2 Engine::ProjectPositionToWindow(Vec2 pos) {

        return (pos * m_Bounds) / SCENE_BOUNDS;

    }

    bool Engine::AddCircle(float mass, float radius, float restituion, float friction, bool isStatic, Vec2 initialPosition, float initialAngle, Color color) {

        m_Scene.push_back(std::make_unique<Circle>(mass, restituion, friction, isStatic, ProjectPositionToWindow(initialPosition), initialAngle, radius * m_Bounds / SCENE_BOUNDS, color));
        
        return true;
    }

    bool Engine::AddBox(float mass, Vec2 dimensions, float restituion, float friction, bool isStatic, Vec2 initialPosition, float initialAngle, Color color) {

        m_Scene.push_back(std::make_unique<Box>(mass, restituion, friction, isStatic, ProjectPositionToWindow(initialPosition), initialAngle, dimensions, color));

        return true;
    }

}
