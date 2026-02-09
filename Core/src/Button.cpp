#include "Button.h"
#include "Utils.h"

Core::Button::Button(int x, int y, onClickedFn callbackFn)
{
	m_Position = { (float)x, (float)y };
	m_BoundingBox = { 0, 0, (float)GetScreenWidth()/50, (float)GetScreenHeight() / 50 };
	m_Texture = Core::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
	OnClicked = callbackFn;
}

Core::Button::~Button()
{
		
	UnloadTexture(m_Texture);

}

bool Core::Button::isHovered(Vector2& mousePos) const
{
	return CheckCollisionPointRec(mousePos, m_BoundingBox);
}

 
