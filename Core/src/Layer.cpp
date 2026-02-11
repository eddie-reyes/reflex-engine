#include "Layer.h"
#include "Application.h"
#include "LayerEvents.h"

void Core::Layer::QueueTransition(std::unique_ptr<Layer> toLayer)
{
	// Acquire the application instance, then call the (non-static) member on that instance.
	Application& app = Application::Get();
	auto& layerStack = app.LayerStack;

	for (auto& layer : layerStack)
	{
		if (layer.get() == this)
		{
			OnLayerTransitionEvent event(toLayer->GetLayerState());
			app.RaiseEvent(event); //raise an event to notify the new layer of the transition
			layer = std::move(toLayer); //swap the layer in the stack with the new one

			return;

		}
	}

}
