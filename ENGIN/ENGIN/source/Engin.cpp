#include <iostream>
#include <GL\glew.h>
#include <SDL\SDL.h>

#include "Engin\Engin.h"

#include "Engin\Renderer\Window.h"

#include "INIReader.h"


namespace Engin
{
	Engin::Engin()
	{
	}

	Engin::~Engin()
	{
		glDeleteVertexArrays(1, &VAO);
		SDL_GL_DeleteContext(glContext);
	}

	void Engin::init(const std::string& path)
	{
		// TODO (eeneku): Get parameters from a file!
		SDL_Init(SDL_INIT_EVERYTHING);

		INIReader reader(path);

		if (reader.ParseError() < 0) 
		{
			std::cout << "Can't load " << path << std::endl;
		}
		else
		{
			std::cout << "Version: " << reader.GetReal("Engine", "version", -1) << std::endl;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

		
		window.createWindow(reader.Get("Window", "title", "unnamed"),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			reader.GetReal("Window", "width", 1280.0f),
			reader.GetReal("Window", "height", 720));

		SDL_SetWindowFullscreen(window.getSDLWindow(), reader.GetBoolean("Window", "fullscreen", false));

		glContext = SDL_GL_CreateContext(window.getSDLWindow());

		glewExperimental = GL_TRUE;
		glewInit();
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);

		glEnable(GL_DEPTH_TEST);


		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	}

	void Engin::quit()
	{
		SDL_Quit();
	}

	int Engin::update()
	{
		window.swapWindow();

		SDL_Event event;
		int returnValue = 1;

		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
			{
				returnValue = 0;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		return returnValue;
	}
}