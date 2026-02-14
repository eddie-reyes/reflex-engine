#include "SimLayer.h"
#include "MenuLayer.h"
#include "Application.h"


SimLayer::SimLayer()
{
	
	m_Buttons.push_back(std::make_unique<Core::Button>([]() { Core::Application::Get().Engine.TogglePause(); }));

	m_Buttons.push_back(std::make_unique<Core::Button>([this]() { Core::Application::Get().Engine.ResetScene(m_SimBounds); }));

	m_Buttons.push_back(std::make_unique<Core::Button>([this]() { 
		Core::Application::Get().Engine.ClearScene();
		TransitionTo<MenuLayer>();
	}));

	m_SimBounds = (float)GetScreenWidth();
	m_IsSceneTickable = true;

	SetRelativePositionOfUI((float)GetScreenWidth(), (float)GetScreenHeight());
	Core::Application::Get().Engine.MapSceneCoordsToWindow(200.0f, m_SimBounds);
}

void SimLayer::OnEvent(Core::Event& event)
{

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return OnWindowResize(e); });

}
void SimLayer::OnRender()
{
	const auto& bodies = Core::Application::Get().Engine.GetSceneBodies();
	TickScene();

    for (const auto& b : bodies) {
        if (b->Shape.Type == Core::Engine::ShapeType::Circle) {

			DrawCircleV({ b->Position.x, b->Position.y }, b->Shape.Radius, b->FillColor);

        }
		if (b->Shape.Type == Core::Engine::ShapeType::AABB) {

			DrawRectangleV({ b->Position.x - b->Shape.Half.x, b->Position.y - b->Shape.Half.y }, { b->Shape.Half.x * 2, b->Shape.Half.y * 2 }, b->FillColor);
		
		}

        if (b->Shape.Type == Core::Engine::ShapeType::OBB) {
        
			DrawRectanglePro({ b->Position.x, b->Position.y, b->Shape.Half.x * 2, b->Shape.Half.y * 2 }, {b->Shape.Half.x, b->Shape.Half.y}, b->Angle * R_TO_DEG_RATIO, b->FillColor);

        }
    }

	for (const auto& button : m_Buttons) {

		DrawTextureRec(button->GetTexture(), button->GetSourceRect(), button->GetPosition(), WHITE);

	}

}

void SimLayer::SetRelativePositionOfUI(int screenWidth, int screenHeight)
{

	for (int i = 0; i < m_Buttons.size(); i++) {
		m_Buttons[i]->SetPosition((screenWidth / 4) * (i + 1), screenHeight - (screenHeight / 6));
	}	
    
}

bool SimLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{
	Vector2 mousePos = GetMousePosition();

	for (const auto& button : m_Buttons) {
		if (button->isHovered(mousePos)) {
			button->OnClicked();
			return true;
		}
	}

	return false;
}


bool SimLayer::OnWindowResize(Core::WindowResizeEvent& event)
{

	Core::Application::Get().Engine.MapSceneCoordsToWindow(m_SimBounds, (float)event.GetWidth());
	m_SimBounds = (float)event.GetWidth();
	SetRelativePositionOfUI((float)event.GetWidth(), (float)event.GetHeight());

	return true;
}


void SimLayer::TickScene() {

	if (!m_IsSceneTickable) return;

	SceneType currentScene = Core::Application::Get().Engine.GetCurrentScene();

	switch (currentScene) {

	case SceneType::BINOMIAL_SCENE:

		m_IsSceneTickable = false;
		
		Core::Application::Get().Engine.AddCircle(1, 1, 1, 1, false, { 100, 0 }, 0, RED);

	}

}
