#include "Button.h"

Core::Button::Button(const char * text, onClickedFn callbackFn, ButtonType type)
{
	m_Texture = AssetManager::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
	m_SourceRect = { 0.0f, 0.0f, (float)m_Texture.width, (float)m_Texture.height }; 
	m_Position = { m_SourceRect.x, m_SourceRect.y };
	m_Text = text;
	m_Type = type;
	m_isToggled = false;
	m_State = ButtonState::DEFAULT;
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

void Core::Button::setState(Vector2& mousePos) {

	if (m_State == ButtonState::DEFAULT && isHovered(mousePos)) {
		m_State = ButtonState::HOVERED;
		m_Texture = AssetManager::LoadAssetTexture(AssetType::BUTTON_TEXTURE_HOVERED);
	}

	if (m_State == ButtonState::HOVERED && !isHovered(mousePos)) {
		m_State = ButtonState::DEFAULT;
		m_Texture = AssetManager::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
	}



}

void Core::Button::ToggleText() {

	m_Text = m_isToggled ? "Pause" : "UnPause";
	m_isToggled = !m_isToggled;

}


 
