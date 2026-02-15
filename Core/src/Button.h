#pragma once

#include "raylib.h"
#include <functional>
#include <string>
#include "AssetManager.h"

enum class ButtonState {

	DEFAULT,
	HOVERED
};

enum class ButtonType {

	DEFAULT,
	TOGGLEABLE

};


auto constexpr FONT_SIZE = 36;

namespace Core {

	
	class Button
	{

		using onClickedFn = std::function<void()>;


	public:
		Button(const char * text, onClickedFn callbackFn, ButtonType type = ButtonType::DEFAULT);
		~Button();

		bool isHovered(Vector2& mousePos) const;

		void setState(Vector2& mousePos);

		void ToggleText();

		Texture2D& GetTexture() { return m_Texture; }

		Rectangle& GetSourceRect() { return m_SourceRect; }
		
		Vector2& GetPosition() { return m_Position; }

		ButtonType GetType() const { return m_Type; };

		const char * GetText() { return m_Text; };

		void SetPosition(float x, float y) { 
			
			m_Position.x = x - (float)(m_Texture.width / 2);
			m_Position.y = y - (float)(m_Texture.height / 2);

		}

		onClickedFn OnClicked;


	private:

		Texture2D m_Texture;

		Rectangle m_SourceRect;

		Vector2 m_Position;

		ButtonState m_State;

		ButtonType m_Type;

		const char * m_Text;

		bool m_isToggled;

	};


}


