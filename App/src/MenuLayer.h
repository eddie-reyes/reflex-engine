#pragma once

#include "Layer.h"
#include "Button.h"
#include "InputEvents.h"


#include <vector>

class MenuLayer : public Core::Layer
{
public:
	MenuLayer();

	virtual void OnEvent(Core::Event& event) override;

	virtual void OnUpdate(float ts) override;

	virtual void OnRender() override;


private:

	void SetRelativePositionOfScene(int screenWidth, int screenHeight);

	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);

	bool OnMouseScrolled(Core::MouseScrolledEvent& event);

	bool OnWindowResize(Core::WindowResizeEvent& event);

	std::vector<std::unique_ptr<Core::Button>> m_Buttons;

};