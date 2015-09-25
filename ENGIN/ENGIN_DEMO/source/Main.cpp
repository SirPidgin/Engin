#include <Engin\Engin.h>
#include <Engin\Renderer\Window.h>
#include <iostream>

#include <GL\glew.h>

int main(int argc, char** argv)
{
	Engin::init();
	Engin::Renderer::Window window;
	window.createWindow("HAHAHAHA", 300, 300, 800, 600, 0);

	Engin::initGlew();

	while (Engin::pollEvents())
	{

	}
	
	Engin::quit();

	return 0;
}