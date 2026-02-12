#include "Layer.h"
#include "Application.h"

void Core::Layer::QueueTransition(std::unique_ptr<Layer> toLayer)
{
	// Acquire the application instance, then call the (non-static) member on that instance.
	Application& app = Application::Get();
	auto& layerStack = app.LayerStack;

	for (auto& layer : layerStack)
	{
		if (layer.get() == this)
		{
			layer = std::move(toLayer); //swap the layer in the stack with the new one

			return;

		}
	}

}
