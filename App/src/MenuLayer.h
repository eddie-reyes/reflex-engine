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

	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);

	std::vector<std::unique_ptr<Core::Button>> m_Buttons;

};