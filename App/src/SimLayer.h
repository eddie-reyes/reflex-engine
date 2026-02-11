#pragma once

#include "Layer.h"
#include "Button.h"
#include "InputEvents.h"
#include "Engine/Body.h"

#include <vector>

class SimLayer : public Core::Layer
{
public:
	SimLayer();

	virtual void OnEvent(Core::Event& event) override;

	virtual void OnRender() override;


private:

	void SetRelativePositionOfScene(int screenWidth, int screenHeight);

	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);

	bool OnWindowResize(Core::WindowResizeEvent& event);


};