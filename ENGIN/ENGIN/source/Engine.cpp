#include "Engine.h"
#include <GL\glew.h>
#include <SDL\SDL.h>

namespace Engin
{
	void init()
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		std::cout << "SDL!" << std::endl;
	}


	void initGlew()
	{
		glewInit();
	}


	void quit()
	{
		SDL_Quit();
		std::cout << "NOT!" << std::endl;
	}


	void helloWorld()
	{
		std::cout << "It's alive! Hello there. My name is Engin." << std::endl;
	}
}