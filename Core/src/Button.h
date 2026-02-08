#include "raylib.h"
#include <functional>

namespace Core {


	
	class Button
	{

		using onClickedFn = std::function<void()>;

	public:
		Button(int x, int y, onClickedFn callbackFn);
		~Button();

		bool isHovered(Vector2& mousePos) const;

		onClickedFn OnClicked;

	private:

		Texture2D m_Texture;

		Vector2 m_Position;

		Rectangle m_BoundingBox;


	};


}


