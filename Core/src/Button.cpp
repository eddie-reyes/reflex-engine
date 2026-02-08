#include "Button.h"

Core::Button::Button(int x, int y, onClickedFn callbackFn)
{
	m_Position = { (float)x, (float)y };
	m_Texture = LoadTexture("assets/button.png");
	OnClicked = callbackFn;
}

Core::Button::~Button()
{
		
	UnloadTexture(m_Texture);

}

bool Core::Button::isHovered(Vector2& mousePos) const
{
	return CheckCollisionPointRec( mousePos, m_BoundingBox);
}

 
