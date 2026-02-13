#pragma once

#include "Layer.h"
#include "Button.h"
#include "InputEvents.h"

#include <vector>

class SimLayer : public Core::Layer
{
public:
	SimLayer();

	virtual void OnEvent(Core::Event& event) override;

	virtual void OnRender() override;


private:

	void SetRelativePositionOfUI(int screenWidth, int screenHeight);

	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);

	bool OnWindowResize(Core::WindowResizeEvent& event);

	std::vector<std::unique_ptr<Core::Button>> m_Buttons;

};