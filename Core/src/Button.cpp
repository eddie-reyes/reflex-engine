#include "Button.h"

namespace Core {

	Button::Button(const char * text, onClickedFn callbackFn, ButtonType type)
	{
		m_Texture = AssetManager::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
		m_SourceRect = { 0.0f, 0.0f, (float)m_Texture.width, (float)m_Texture.height }; 
		m_Position = { 0, 0 };
		m_Text = m_DefaultText = text;
		m_Type = type;
		m_isToggled = false;
		m_State = ButtonState::DEFAULT;
		OnClicked = callbackFn;

	}

	Button::~Button()
	{
		//UnloadTexture(m_Texture);
	}

	bool Button::isHovered(Vector2& mousePos) const
	{
		return CheckCollisionPointRec(mousePos, { m_Position.x, m_Position.y, m_SourceRect.width, m_SourceRect.height });

	}

	void Button::setState(Vector2& mousePos) {

		if (m_State == ButtonState::DEFAULT && isHovered(mousePos)) {
			m_State = ButtonState::HOVERED;
			m_Texture = AssetManager::LoadAssetTexture(AssetType::BUTTON_TEXTURE_HOVERED);
		}

		if (m_State == ButtonState::HOVERED && !isHovered(mousePos)) {
			m_State = ButtonState::DEFAULT;
			m_Texture = AssetManager::LoadAssetTexture(AssetType::BUTTON_TEXTURE);
		}



	}

	void Button::ToggleText() {

		m_Text = m_isToggled ? m_DefaultText : "UnPause";
		m_isToggled = !m_isToggled;

	}

	void Button::SetText(const char* text) {


		if (m_Type == ButtonType::TOGGLEABLE && m_Text == m_DefaultText) {

			m_isToggled = false;
			return;

		}

		m_Text = text;

	}

}
