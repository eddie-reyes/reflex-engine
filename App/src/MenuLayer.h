#pragma once

#include "Layer.h"
#include <vector>
#include "Button.h"

class MenuLayer : public Core::Layer
{
public:
	MenuLayer();
	virtual ~MenuLayer();

	virtual void OnEvent(Core::Event& event) override;

	virtual void OnUpdate(float ts) override;
private:

	std::vector<std::unique_ptr<Core::Button>> m_Buttons;

};