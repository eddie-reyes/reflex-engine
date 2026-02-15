#include "MenuLayer.h"
#include "SimLayer.h"
#include "Application.h"

#define SCROLL_SPEED 10.0f

MenuLayer::MenuLayer()
{

	m_Buttons.push_back(std::make_unique<Core::Button>("View", [this]() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }));
	m_Buttons.push_back(std::make_unique<Core::Button>("View", [this]() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }));
	m_Buttons.push_back(std::make_unique<Core::Button>("View", [this]() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }));
	m_Buttons.push_back(std::make_unique<Core::Button>("View", [this]() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }));

	SetRelativePositionOfUI(GetScreenWidth(), GetScreenHeight());
	 
}

void MenuLayer::OnEvent(Core::Event& event)
{

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) { return OnMouseMoved(e); });
	dispatcher.Dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) { return OnMouseScrolled(e); });
	dispatcher.Dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return OnWindowResize(e); });
	 
}

void MenuLayer::OnRender()
{

	for (const auto& button : m_Buttons) {

		DrawTextureRec(button->GetTexture(), button->GetSourceRect(), button->GetPosition(), WHITE);
		DrawText(button->GetText(), button->GetPosition().x + (button->GetTexture().width / 2 - MeasureText(button->GetText(), FONT_SIZE) / 2), button->GetPosition().y + button->GetTexture().height / 4, FONT_SIZE, WHITE);
	}

}

void MenuLayer::SetRelativePositionOfUI(int screenWidth, int screenHeight)
{

	for (int i = 0; i < m_Buttons.size(); i++) {
		m_Buttons[i]->SetPosition((screenWidth / 3) * (i % 2 + 1), (screenHeight / 3) * (i / 2 + 1));
	}

}

bool MenuLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{
	Vector2 mousePos = GetMousePosition();

	for (const auto& button : m_Buttons) {
		if (button->isHovered(mousePos)) {

			button->OnClicked();
			TransitionTo<SimLayer>();

			return true;
		}
	}

	return false;
}

bool MenuLayer::OnMouseMoved(Core::MouseMovedEvent& e)
{
	Vector2 mousePos = { e.GetX(), e.GetY() };

	for (const auto& button : m_Buttons) {
		button->setState(mousePos);
	}

	return true;
}

bool MenuLayer::OnMouseScrolled(Core::MouseScrolledEvent& event)
{
	for (auto& button : m_Buttons) {
		button->GetPosition().y += (event.GetDirection() * SCROLL_SPEED);
	}

	return true;
}

bool MenuLayer::OnWindowResize(Core::WindowResizeEvent& event)
{

	SetRelativePositionOfUI(event.GetWidth(), event.GetHeight());

	return true;
}