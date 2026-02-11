#include "SimLayer.h"
#include "Engine/Collision.h"
#include "Engine/Impulse.h"

static Core::Engine::Vec2 gravity = { 0, 9.8f };

SimLayer::SimLayer()
{
	m_ScreenSize = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    m_bodies.push_back(std::make_unique<Core::Engine::Circle>(10, 0.5, 0.1, 50));
    m_bodies.push_back(std::make_unique<Core::Engine::Circle>(10, 0.5, 0.1, 50));
	Core::Engine::Vec2 boundingBox = { (float)GetScreenWidth(), 100};
	m_bodies.push_back(std::make_unique<Core::Engine::Box>(STATIC_BODY, 1, 0.1, boundingBox));
    m_bodies[0]->Position = { (float)GetScreenWidth() / 2 - 25.0f , (float)GetScreenHeight() / 3 };
    m_bodies[1]->Position = { (float)GetScreenWidth() / 2 , (float)GetScreenHeight() / 2};
	m_bodies[2]->Position = { (float)GetScreenWidth() / 2 , (float)GetScreenHeight() - 50 };
}

void SimLayer::OnEvent(Core::Event& event)
{

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return OnWindowResize(e); });

}

void SimLayer::OnUpdate(float dt)
{
    // 1) apply forces + integrate velocities
    for (auto& b : m_bodies) {
		if (b->IsStatic()) continue; //ignore static bodies

        b->Velocity += gravity * dt;

    }

    // 2) build contacts
    std::vector<Core::Engine::Manifold> contacts;
    contacts.reserve(m_bodies.size());

    const int n = (int)m_bodies.size();
    for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) { //narrowphase
            Core::Engine::Body& A = *m_bodies[i];
            Core::Engine::Body& B = *m_bodies[j];
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
            ApplyImpulse(*m_bodies[c.ia], *m_bodies[c.ib], c);
        }
    }

    // 4) integrate positions
    for (auto& b : m_bodies) {
        if (b->IsStatic()) continue;
        b->Position += b->Velocity * dt;
    }

    // 5) positional correction
    for (auto& c : contacts) {
        PositionalCorrection(*m_bodies[c.ia], *m_bodies[c.ib], c);
    }
}

void SimLayer::OnRender()
{
    for (auto& b : m_bodies) {
        if (b->Shape.Type == Core::Engine::ShapeType::Circle) {

			DrawCircleV({ b->Position.x, b->Position.y }, b->Shape.Radius, RED);

        }

        if (b->Shape.Type == Core::Engine::ShapeType::AABB) {
        
			DrawRectangleV({ b->Position.x - b->Shape.Half.x , b->Position.y - b->Shape.Half.y }, {b->Shape.Half.x * 2, b->Shape.Half.y * 2}, RED);
        }
    }
	

}

void SimLayer::SetRelativePositionOfScene(int screenWidth, int screenHeight)
{
	for (auto& b : m_bodies) {
        b->Position.x = b->Position.x * (screenWidth * m_ScreenSize.x);
        b->Position.y = b->Position.y * (screenHeight * m_ScreenSize.y);
	}

    m_ScreenSize = { (float)screenWidth, (float)screenHeight };
}

bool SimLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{
	Vector2 mousePos = GetMousePosition();

	return false;
}


bool SimLayer::OnWindowResize(Core::WindowResizeEvent& event)
{
	SetRelativePositionOfScene(event.GetWidth(), event.GetHeight());

	return true;
}
