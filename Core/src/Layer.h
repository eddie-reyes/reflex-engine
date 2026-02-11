#pragma once

#include "Event.h"

#include <memory>

namespace Core {

	enum class LayerState {SimEnabled, SimDisabled, SimActive, SimPaused};

	class Layer
	{
	public:

		virtual ~Layer() = default;

		virtual void OnEvent(Event& event) = 0;

		virtual void OnRender() = 0;

		LayerState GetLayerState() const { return m_LayerState; }
		
		template<std::derived_from<Layer> TLayer>
		void TransitionTo()
		{
			QueueTransition(std::move(std::make_unique<TLayer>()));
		}

	protected:
		LayerState m_LayerState = LayerState::SimDisabled;

	private:
		void QueueTransition(std::unique_ptr<Layer> layer);

	};

}