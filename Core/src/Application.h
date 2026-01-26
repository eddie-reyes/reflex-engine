#pragma once

#include "raylib.h"
#include <cassert>
#include <stack>
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

		Application& GetInstance() const;
		double GetTime() const;

		void Run();
		void Stop();


	private:

		bool m_IsRunning = false;
		WindowProperties m_WindowProperties;

	};



}