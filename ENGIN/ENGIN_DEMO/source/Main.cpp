#include <Engin\Engin.h>
#include <Engin\Renderer\Window.h>
#include <iostream>

#include <GL\glew.h>

int main(int argc, char** argv)
{
	Engin::init();
	Engin::Renderer::Window window;
	window.createWindow("HAHAHAHA", 300, 300, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	Engin::initGlew();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window.getWindow());

	while (Engin::pollEvents())
	{

	}
	
	Engin::quit();

	return 0;
}