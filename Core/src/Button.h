#pragma once

#include "raylib.h"
#include <functional>

namespace Core {

	
	class Button
	{

		using onClickedFn = std::function<void()>;


	public:
		Button(onClickedFn callbackFn);
		~Button();

		bool isHovered(Vector2& mousePos) const;

		Texture2D& GetTexture() { return m_Texture; }

		Rectangle& GetSourceRect() { return m_SourceRect; }
		
		Vector2& GetPosition() { return m_Position; }

		void SetPosition(float x, float y) { 
			
			m_Position.x = x - (float)(m_Texture.width / 2);
			m_Position.y = y - (float)(m_Texture.height / 2);

		}

		onClickedFn OnClicked;

	private:

		Texture2D m_Texture;

		Rectangle m_SourceRect;

		Vector2 m_Position;


	};


}


