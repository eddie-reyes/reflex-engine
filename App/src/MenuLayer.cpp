#include "MenuLayer.h"
#include "SimLayer.h"
#include "Application.h"

#define SCROLL_SPEED 10.0f

MenuLayer::MenuLayer()
{

	m_Cards.push_back(std::make_unique<Core::Card>(std::make_unique<Core::Button>("View", []() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }), Core::AssetManager::LoadAssetTexture(AssetType::BINOMIAL_SCENE_IMG), "Binomial Plinko"));
	/*m_Cards.push_back(std::make_unique<Core::Card>(Core::Button("View", []() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }), Core::AssetManager::LoadAssetTexture(AssetType::BINOMIAL_SCENE_IMG), "Binomial Plinko"));*/
	//m_Cards.push_back(std::make_unique<Core::Button>("View", [this]() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }));
	//m_Cards.push_back(std::make_unique<Core::Button>("View", [this]() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }));
	//m_Cards.push_back(std::make_unique<Core::Button>("View", [this]() { Core::Application::Get().Engine.BuildScene(SceneType::BINOMIAL_SCENE);  }));

	SetRelativePositionOfUI(GetScreenWidth(), GetScreenHeight());
	 
}

void MenuLayer::OnEvent(Core::Event& event)
{

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) { return OnMouseButtonPressed(e); });
	dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) { return OnMouseMoved(e); });
	dispatcher.Dispatch<Core::MouseScrolledEvent>([this](Core::MouseScrolledEvent& e) { return OnMouseScrolled(e); });
	dispatcher.Dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return OnWindowResize(e); });
	 
}

void MenuLayer::OnRender()
{

	for (const auto& card : m_Cards) {

		card->DrawCard();
	}

}

void MenuLayer::SetRelativePositionOfUI(int screenWidth, int screenHeight)

{
	for (int i = 0; i < m_Cards.size(); i++) {

		Vector2 cardPos = { (float)(screenWidth / 3) * (i % 2 + 1), (float)(screenHeight / 3) * (i / 2 + 1) };
		m_Cards[i]->SetPosition(cardPos);
	}

}

bool MenuLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{
	Vector2 mousePos = GetMousePosition();

	for (const auto& card : m_Cards) {
		if (card->GetButton()->isHovered(mousePos)) {

			card->GetButton()->OnClicked();
			TransitionTo<SimLayer>();

			return true;
		}
	}

	return false;
}

bool MenuLayer::OnMouseMoved(Core::MouseMovedEvent& e)
{
	Vector2 mousePos = { e.GetX(), e.GetY() };

	for (const auto& card : m_Cards) {
		card->GetButton()->setState(mousePos);
	}

	return true;
}

bool MenuLayer::OnMouseScrolled(Core::MouseScrolledEvent& event)
{
	for (auto& card : m_Cards) {
		//card->GetButton().GetPosition().y += (event.GetDirection() * SCROLL_SPEED);
	}

	return true;
}

bool MenuLayer::OnWindowResize(Core::WindowResizeEvent& event)
{

	SetRelativePositionOfUI(event.GetWidth(), event.GetHeight());

	return true;
}