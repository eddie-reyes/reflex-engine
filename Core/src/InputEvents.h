#include "Event.h"

namespace Core {

	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {
		}

		int m_KeyCode;
	};
}