#pragma once

#include "raylib.h"
#include <functional>
#include <string>

auto constexpr FONT_SIZE = 36;

namespace Core {

	
	class Button
	{

		using onClickedFn = std::function<void()>;


	public:
		Button(const char * text, onClickedFn callbackFn);
		~Button();

		bool isHovered(Vector2& mousePos) const;

		Texture2D& GetTexture() { return m_Texture; }

		Rectangle& GetSourceRect() { return m_SourceRect; }
		
		Vector2& GetPosition() { return m_Position; }

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

		const char * m_Text;

	};


}


