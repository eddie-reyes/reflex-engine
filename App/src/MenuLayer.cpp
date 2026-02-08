#include "MenuLayer.h"

MenuLayer::MenuLayer()
{

	m_Buttons.push_back(std::make_unique<Core::Button>(100, 100, [this]() { TransitionTo<MenuLayer>(); }));


}

MenuLayer::~MenuLayer()
{



}

void MenuLayer::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);


}

void MenuLayer::OnUpdate(float ts)
{
	Vector2 mousePos = GetMousePosition();

	for (auto& button : m_Buttons) {

		if (button->isHovered(mousePos)) {

			button->OnClicked();
		}
	}
}
