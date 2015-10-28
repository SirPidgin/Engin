#include <iostream>
#include <algorithm>
#include <GL\glew.h>
#include <SDL\SDL.h>

#include "Engin\Engin.h"

#include "Engin\Renderer\Window.h"

#include "inih\INIReader.h"


namespace Engin
{
	Engin::Engin() : running(true), accumulator(0.0f), step(0.0f)
	{
	}

	Engin::~Engin()
	{
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
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, reader.GetInteger("Graphics", "antialiasing", 4));
		
		window.createWindow(reader.Get("Window", "title", "unnamed"),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			reader.GetInteger("Window", "width", 1280),
			reader.GetInteger("Window", "height", 720));

		SDL_SetWindowFullscreen(window.getSDLWindow(), reader.GetBoolean("Window", "fullscreen", false));

		step = 1.0f / (float)reader.GetReal("Physics", "step", 60.0f);

		glContext = SDL_GL_CreateContext(window.getSDLWindow());

		glewExperimental = GL_TRUE;
		glewInit();
		glClearColor(0.2f, 0.4f, 0.6f, 1.0f);

		glEnable(GL_DEPTH_TEST);
	}

	void Engin::quit()
	{
		SDL_Quit();
	}

	void Engin::run(Game::Scene* scene)
	{
		// Variables for deltatime
		float deltaTime = 0.0f;
		float newTime = 0.0f;
		float currentTime = SDL_GetTicks() / 1000.0f; // TODO (eeneku): Use own hi-res timer.

		sceneManager.change(scene);

		while (running)
		{
			newTime = SDL_GetTicks() / 1000.0f;
			deltaTime = std::min(newTime - currentTime, 0.25f);
			currentTime = newTime;

			handleEvents();
			update(step);
			draw();
		}
	}

	void Engin::handleEvents()
	{
		// TODO (eeneku): This method is temporary.
		SDL_Event event;

		while (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}
	}

	void Engin::update(float deltaTime)
	{

		accumulator += deltaTime;

		while (accumulator >= step)
		{
			// TODO (eeneku): Maybe we can only pass alpha (accumulator / step)?
			sceneManager.update(step);
			accumulator -= step;
		}

		// TODO (eeneku): What about interpolation? Probably handled in Scene or SceneManager.
		sceneManager.interpolate(accumulator / step);
	}

	void Engin::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneManager.draw();

		window.swapWindow();
	}
}