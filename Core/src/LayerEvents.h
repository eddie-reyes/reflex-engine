#include "Event.h"
#include "Layer.h"

#include <format>

namespace Core
{

	class OnLayerTransitionEvent : public Event
	{
	public:
		OnLayerTransitionEvent(LayerState LayerState) : Event(), m_ToLayerState(LayerState) {}

		std::string ToString() const override
		{
			return std::format("OnTransitionEvent: {}", GetName() );
		}

		EVENT_CLASS_TYPE(LayerTransition);
	private:

		LayerState m_ToLayerState;
	};
};