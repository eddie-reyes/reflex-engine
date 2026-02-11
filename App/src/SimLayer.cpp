#include "SimLayer.h"
#include "Application.h"


SimLayer::SimLayer()
{
	m_LayerState = Core::LayerState::SimPaused;

}

void SimLayer::OnEvent(Core::Event& event)
{

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return OnWindowResize(e); });

}
void SimLayer::OnRender()
{
	const auto& bodies = Core::Application::Get().Engine.GetScene();

    for (auto& b : bodies) {
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
