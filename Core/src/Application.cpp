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

	Application& Application::GetInstance() const {

		assert(s_Instance, "Invalid Application instance");
		return *s_Instance;
	}

	void Application::Run() {

		m_IsRunning = true;

		//initialize raylib window with spec
		InitWindow(m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.Title);

		while (!WindowShouldClose() && m_IsRunning) {

			BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
			EndDrawing();

		}
	}

	void Application::Stop() {

		m_IsRunning = false;

	}

	double Application::GetTime() const {
		return GetTime();

	}
}
