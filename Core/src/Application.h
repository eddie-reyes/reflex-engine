#pragma once

#include "raylib.h"
#include "Layer.h"
#include <cassert>
#include <deque>
#include <memory>

namespace Core {

	struct WindowProperties {
		const char* Title;
		int Width;
		int Height;

	};

	class Application {

	public:
		Application(const WindowProperties& windowProps);
		~Application();

		static Application& GetInstance();
		
		void Run();
		void Stop();

		std::deque<std::unique_ptr<Layer>> LayerStack;

		void RaiseEvent(Event& event);

		template<std::derived_from<Layer> T>
		void PushLayer()
		{
			LayerStack.push_front(std::make_unique<T>());
		}

		
	private:

		bool m_IsRunning = false;
		WindowProperties m_WindowProperties;


	};



}