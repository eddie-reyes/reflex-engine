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
		
		Rectangle& GetBoundingBox() { return m_BoundingBox; }

		void SetPosition(float x, float y) { 
			
			m_BoundingBox.x = x - (float)m_Texture.width/2;
			m_BoundingBox.y = y - (float)m_Texture.height / 2;
		}

		onClickedFn OnClicked;

	private:

		Texture2D m_Texture;

		Rectangle m_SourceRect;

		Rectangle m_BoundingBox;


	};


}


