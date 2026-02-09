#pragma once

#include "Event.h"

#include <memory>

namespace Core {

	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& event) = 0;

		virtual void OnUpdate(float ts) = 0;

		virtual void OnRender() = 0;

		template<std::derived_from<Layer> TLayer>
		void TransitionTo()
		{
			QueueTransition(std::move(std::make_unique<TLayer>()));
		}
	private:
		void QueueTransition(std::unique_ptr<Layer> layer);
	};

}