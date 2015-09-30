#include <Engin\Engin.h>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Camera.h>
#include <iostream>

#include <GL\glew.h>

int main(int argc, char** argv)
{
	Engin::init();
	Engin::Renderer::Window window;
	Engin::Renderer::Camera camera;

	window.createWindow("HAHAHAHA", 300, 300, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	camera.initCamera(window.getWindow(), 0.0f, 0.0f, 256.0f);

	Engin::initGlew();

	while (Engin::update())
	{
		Engin::swapWindow(window);
	}
	
	Engin::quit();

	return 0;
}