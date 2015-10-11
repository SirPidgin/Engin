#include <Engin\Engin.h>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Camera.h>
#include <iostream>

int main(int argc, char** argv)
{
	Engin::init();
	Engin::Renderer::Window window;
	Engin::Renderer::Camera camera;

	window.createWindow("HAHAHAHA", 300, 300, 800, 600);
	camera.initCamera(800, 600, 0.0f, 0.0f, 256, 0, 0, 800, 600);

	Engin::initGlew();

	while (Engin::update())
	{
		window.swapWindow();
	}
	
	Engin::quit();

	return 0;
}