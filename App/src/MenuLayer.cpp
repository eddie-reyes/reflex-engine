#include "MenuLayer.h"
#include <iostream>

MenuLayer::MenuLayer()
{

	m_Buttons.push_back(std::make_unique<Core::Button>([this]() { std::cout << "button pressed" << std::endl; }));

	float width = GetScreenWidth();
	float height = GetScreenHeight();
	float padding = width / 3;

	for (int i = 0; i < m_Buttons.size(); i++) { 
		m_Buttons[i]->SetPosition(width / 2, height / 2);
	}


}

void MenuLayer::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
		
}

void MenuLayer::OnUpdate(float ts)
{
	
}

void MenuLayer::OnRender()
{

	BeginDrawing();

	for (const auto& button : m_Buttons) {

		ClearBackground(RAYWHITE);
		DrawTextureRec(button->GetTexture(), button->GetSourceRect(), {button->GetBoundingBox().x, button->GetBoundingBox().y}, WHITE);
		
	}

	EndDrawing();
}

bool MenuLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
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
