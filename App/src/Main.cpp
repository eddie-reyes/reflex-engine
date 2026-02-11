#include "Application.h"
#include "MenuLayer.h"
#include "SimLayer.h"

int main() {

	Core::WindowProperties windowProps;
	windowProps.Title = "Reflex Engine";
	windowProps.Width = 1920;
	windowProps.Height = 1080;

	Core::Application app(windowProps);
	app.PushLayer<SimLayer>();
	app.PushLayer<MenuLayer>();
	app.Run();


	
	return 0;
}