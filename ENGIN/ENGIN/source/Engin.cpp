#include <iostream>
#include <GL\glew.h>
#include <SDL\SDL.h>

#include "Engin\Engin.h"

namespace Engin
{
	void init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
	}


	void initGlew()
	{
		glewInit();
	}


	void quit()
	{
		SDL_Quit();
	}


	int pollEvents()
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

		return returnValue;
	}
}