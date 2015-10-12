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
	}


	void initGlew()
	{
		glewInit();
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
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
}