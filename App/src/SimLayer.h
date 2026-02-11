#pragma once

#include "Layer.h"
#include "Button.h"
#include "InputEvents.h"
#include "Engine/Body.h"

#include <vector>

#define SOLVER_ITER 10

class SimLayer : public Core::Layer
{
public:
	SimLayer();

	virtual void OnEvent(Core::Event& event) override;

	virtual void OnUpdate(float ts) override;

	virtual void OnRender() override;


private:

	void SetRelativePositionOfScene(int screenWidth, int screenHeight);

	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);

	bool OnWindowResize(Core::WindowResizeEvent& event);

	std::vector<std::unique_ptr<Core::Engine::Body>> m_bodies;

	Vector2 m_ScreenSize;


};