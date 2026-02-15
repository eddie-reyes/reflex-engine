#include "Button.h"
#include "AssetManager.h"

Core::Button::Button(const char * text, onClickedFn callbackFn)
{
	m_Texture = AssetManager::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
	m_SourceRect = { 0.0f, 0.0f, (float)m_Texture.width, (float)m_Texture.height }; 
	m_Position = { m_SourceRect.x, m_SourceRect.y };
	m_Text = text;
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

 
