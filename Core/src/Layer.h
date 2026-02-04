#pragma once

#include "Event.h"

#include <memory>

namespace Core {

	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnEvent(Event& event) {}

		virtual void OnUpdate(float ts) {}
		virtual void OnRender() {}

		template<std::derived_from<Layer> T>
		void TransitionTo()
		{
			QueueTransition(std::move(std::make_unique<T>()));
		}
	private:
		void QueueTransition(std::unique_ptr<Layer> layer);
	};

}