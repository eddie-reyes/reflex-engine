#include "Button.h"
#include "Utils.h"

Core::Button::Button(int x, int y, onClickedFn callbackFn)
{
	m_Position = { (float)x, (float)y };
	m_Texture = Core::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
	m_BoundingBox = { 0, 0, (float)m_Texture.width, (float)m_Texture.height};
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

 
