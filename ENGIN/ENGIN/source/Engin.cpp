#include <iostream>
#include <GL\glew.h>
#include <SDL\SDL.h>

#include "Engin\Engin.h"

#include "Engin\Renderer\Window.h"


namespace Engin
{
	void init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		glClearColor(0, 0, 0, 1);
	}


	void initGlew()
	{
		glewInit();
	}


	void quit()
	{
		SDL_Quit();
	}


	int update()
	{
		SDL_Event event;
		int returnValue = 1;

		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
			{
				returnValue = 0;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		return returnValue;
	}

	void swapWindow(Renderer::Window& window)
	{
		SDL_GL_SwapWindow(window.getWindow());
	}
}