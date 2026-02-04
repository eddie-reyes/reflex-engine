#include "Application.h"

namespace Core {


	static Application* s_Instance = nullptr;


	Application::Application(const WindowProperties& windowProps) {


		m_WindowProperties = windowProps;

		s_Instance = this;

	}

	Application::~Application() {

		CloseWindow();

	}

	Application& Application::GetInstance() {

		assert(s_Instance);
		return *s_Instance;	
	}


	void Application::Run() {

		m_IsRunning = true;

		//initialize raylib window with spec
		InitWindow(m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.Title);

		float lastTime = GetTime();


		while (!WindowShouldClose() && m_IsRunning) {

			PollInputEvents();

			if (WindowShouldClose()) {

				Stop();
				break;
			}

			float currentTime = GetTime();
			float timeStep = currentTime - lastTime;

			for (const std::unique_ptr<Layer>& layer : LayerStack) {

				layer->OnUpdate(timeStep);

			}

			lastTime = currentTime;

		}
	}

	void Application::Stop() {

		m_IsRunning = false;

	}

}
