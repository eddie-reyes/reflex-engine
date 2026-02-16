#pragma once

#include "Layer.h"
#include "Card.h"
#include "InputEvents.h"


#include <vector>

class MenuLayer : public Core::Layer
{
public:
	MenuLayer();

	virtual void OnEvent(Core::Event& event) override;

	virtual void OnRender() override;


private:

	void SetRelativePositionOfUI(int screenWidth, int screenHeight);

	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);

	bool OnMouseMoved(Core::MouseMovedEvent& e);

	bool OnMouseScrolled(Core::MouseScrolledEvent& event);

	bool OnWindowResize(Core::WindowResizeEvent& event);

	std::vector<std::unique_ptr<Core::Card>> m_Cards;

	int m_SceneHeight{0};

};