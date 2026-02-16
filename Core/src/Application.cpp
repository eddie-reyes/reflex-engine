#include "Application.h"
#include "InputEvents.h"
#include "AssetManager.h"
#include <ranges>
#include <iostream>

#define MIN_SCREEN_WIDTH 1280
#define MIN_SCREEN_HEIGHT 720
#define TARGET_FPS 60

namespace Core {

	static Application* s_Instance = nullptr;

	Application::Application(const WindowProperties& windowProps) {

		m_WindowProperties = windowProps;

		s_Instance = this;

		//initialize raylib window with spec
		SetConfigFlags(FLAG_WINDOW_RESIZABLE);
		InitWindow(m_WindowProperties.Width, m_WindowProperties.Height, m_WindowProperties.Title);
		SetWindowMinSize(MIN_SCREEN_WIDTH, MIN_SCREEN_HEIGHT);
		SetTargetFPS(TARGET_FPS);

		Core::AssetManager::LoadTextures();
	}

	Application::~Application() {

		CloseWindow();

		s_Instance = nullptr;

	}

	Application& Application::Get() {

		assert(s_Instance);
		return *s_Instance;
	}


	void Application::Run() {

		m_IsRunning = true;

		while (m_IsRunning) {

			HandleInput();

			if (WindowShouldClose()) {

				Stop();
				break;
			}

			//simulate scene
			Engine.Tick(GetFrameTime());

			BeginDrawing();
			ClearBackground(RAYWHITE);

			//render from bottom -> top (reverse order of layers)
			for (const auto& layer : std::views::reverse(LayerStack)) {

				layer->OnRender();

			}

			EndDrawing();

		}
	}

	void Application::RaiseEvent(Event& event) {

		//propigate top -> down
		for (auto& layer : LayerStack) {

			layer->OnEvent(event);

			if (event.isHandled) {
				break;
			}
		}
	}

	void Application::HandleInput() {
	
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

			MouseButtonPressedEvent event(MOUSE_BUTTON_LEFT);
			RaiseEvent(event);
		}

		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {

			MouseButtonPressedEvent event(MOUSE_BUTTON_RIGHT);
			RaiseEvent(event);
		}

		if (GetMouseWheelMove() != 0) {

			MouseScrolledEvent event(GetMouseWheelMove());
			RaiseEvent(event);
		}

		if (IsWindowResized()) {

			WindowResizeEvent event(GetScreenWidth(), GetScreenHeight());
			RaiseEvent(event);
		}

		if (GetMouseDelta().x != 0) {

			MouseMovedEvent event(GetMousePosition().x, GetMousePosition().y);
			RaiseEvent(event);

		}

	
	}

	void Application::Stop() {

		m_IsRunning = false;

	}


}
