#include "Button.h"
#include "Utils.h"

Core::Button::Button(onClickedFn callbackFn)
{
	m_Texture = Core::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
	m_SourceRect = { 0.0f, 0.0f, (float)m_Texture.width, (float)m_Texture.height };
	m_Position = { 0.0f, 0.0f };
	OnClicked = callbackFn;
}

Core::Button::~Button()
{
	UnloadTexture(m_Texture);
}

bool Core::Button::isHovered(Vector2& mousePos) const
{
	return CheckCollisionPointRec(mousePos, { m_Position.x, m_Position.y, m_SourceRect.width, m_SourceRect.height });

}

 
