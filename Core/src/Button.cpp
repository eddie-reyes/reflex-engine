#include "Button.h"
#include "Utils.h"

Core::Button::Button(onClickedFn callbackFn)
{
	m_Texture = Core::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
	m_SourceRect = { 0, 0, (float)m_Texture.width, (float)m_Texture.height };
	m_BoundingBox = m_SourceRect;
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

 
