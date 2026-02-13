#include "MenuLayer.h"
#include "SimLayer.h"
#include "Application.h"

#define SCROLL_SPEED 10.0f

MenuLayer::MenuLayer()
{

	m_Buttons.push_back(std::make_unique<Core::Button>([this]() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }));

	SetRelativePositionOfUI(GetScreenWidth(), GetScreenHeight());

}

void MenuLayer::OnEvent(Core::Event& event)
{

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) { return OnMouseScrolled(e); });
	dispatcher.Dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return OnWindowResize(e); });
	 
}

void MenuLayer::OnRender()
{

	for (const auto& button : m_Buttons) {

		DrawTextureRec(button->GetTexture(), button->GetSourceRect(), button->GetPosition(), WHITE);

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