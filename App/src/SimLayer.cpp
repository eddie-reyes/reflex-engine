#include "SimLayer.h"
#include "MenuLayer.h"
#include "Application.h"
#include <iostream>

SimLayer::SimLayer()
{
	
	//pause button
	m_Buttons.push_back(std::make_unique<Core::Button>("Pause", []() {
		Core::Application::Get().Engine.TogglePause();
	}, ButtonType::TOGGLEABLE));

	//reset button
	m_Buttons.push_back(std::make_unique<Core::Button>("Reset", [this]() {
		Core::Application::Get().Engine.ResetScene(); 
		m_IsSceneTickable = true;

		auto it = m_Buttons.begin();
		it = std::find_if(m_Buttons.begin(), m_Buttons.end(), [](std::unique_ptr<Core::Button>& b) { return b->GetType() == ButtonType::TOGGLEABLE; });
		(*it)->SetText((*it)->GetDefaultText());

	}));

	//back to menu button
	m_Buttons.push_back(std::make_unique<Core::Button>("To Menu", [this]() {
		Core::Application::Get().Engine.ClearScene();
		TransitionTo<MenuLayer>();
	})); 

	m_IsSceneTickable = true;

	SetRelativePositionOfUI((float)GetScreenWidth(), (float)GetScreenHeight());
	Core::Application::Get().Engine.MapSceneCoordsToWindow(SCENE_BOUNDS, (float)GetScreenWidth());
}

void SimLayer::OnEvent(Core::Event& event)
{

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) { return OnMouseMoved(e); });
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
		DrawText(button->GetText(), button->GetPosition().x + (button->GetTexture().width / 2 - MeasureText(button->GetText(), FONT_SIZE) / 2) , button->GetPosition().y + button->GetTexture().height / 4, FONT_SIZE, WHITE);

	}

}

void SimLayer::SetRelativePositionOfUI(int screenWidth, int screenHeight)
{

	for (int i = 0; i < m_Buttons.size(); i++) {
		m_Buttons[i]->SetPosition((screenWidth / 4) * (i + 1), screenHeight - m_Buttons[i]->GetSourceRect().height);
	}	
    
}

bool SimLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{
	Vector2 mousePos = GetMousePosition();
	
	for (const auto& button : m_Buttons) {
		if (button->isHovered(mousePos)) {
			if (button->GetType() == ButtonType::TOGGLEABLE) button->ToggleText();
			button->OnClicked();
			return true;
		}
	}

	return false;
}

bool SimLayer::OnMouseMoved(Core::MouseMovedEvent& e)
{
	Vector2 mousePos = {e.GetX(), e.GetY()};

	for (const auto& button : m_Buttons) {
		button->setState(mousePos);
	}

	return true;
}


bool SimLayer::OnWindowResize(Core::WindowResizeEvent& event)
{

	Core::Application::Get().Engine.MapSceneCoordsToWindow(Core::Application::Get().Engine.GetBounds(), (float)event.GetWidth());
	SetRelativePositionOfUI((float)event.GetWidth(), (float)event.GetHeight());

	return true;
}


void SimLayer::TickScene() {

	if (!m_IsSceneTickable) return;

	SceneType currentScene = Core::Application::Get().Engine.GetCurrentScene();

	switch (currentScene) {

	case SceneType::BINOMIAL_SCENE:

		m_TimeAccumulator += GetFrameTime();

		if (m_TimeAccumulator >= 0.5) {
			m_TimeAccumulator = 0.0;

			Core::Application::Get().Engine.AddCircle(1, 1, 0.5, 1, false, { (float)GetRandomValue(95, 105), 0}, 0, RED);
				
		}
		

	}

}
