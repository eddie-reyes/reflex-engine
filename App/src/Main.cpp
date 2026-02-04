#include "Application.h"

int main() {

	Core::WindowProperties windowProps;
	windowProps.Title = "Reflex Engine";
	windowProps.Width = 1280;
	windowProps.Height = 720;

	Core::Application app(windowProps);
	app.Run();
	
	return 0;
}