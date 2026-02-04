#pragma once

#include <string>
#include <functional>

namespace Core
{

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased
	};


	class Event
	{
	public:

		bool isHandled = false;
		virtual EventType GetEventType() const = 0;

		virtual ~Event() {}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (!m_Event.isHandled)
			{
				m_Event.isHandled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

}