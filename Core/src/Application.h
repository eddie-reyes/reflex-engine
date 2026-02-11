#pragma once

#include "raylib.h"
#include "Layer.h"
#include "Engine/Engine.h"
#include <cassert>
#include <deque>
#include <memory>

namespace Core {

	using ReflexEngine = Engine::Engine;

	struct WindowProperties {
		const char* Title;
		int Width;
		int Height;

	};

	class Application {

	public:
		Application(const WindowProperties& windowProps);
		~Application();

		static Application& Get();
		
		void Run();
		void Stop();

		void RaiseEvent(Event& event);
		void HandleInput();

		template<std::derived_from<Layer> T>
		void PushLayer()
		{
			LayerStack.push_front(std::make_unique<T>());
		}

		std::deque<std::unique_ptr<Layer>> LayerStack;

		//init engine
		ReflexEngine Engine;

	private:

		bool m_IsRunning = false;
		WindowProperties m_WindowProperties;


	};



}