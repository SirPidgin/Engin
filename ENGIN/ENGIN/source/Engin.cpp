#include <iostream>
#include <algorithm>
#include <GL\glew.h>
#include <SDL\SDL_ttf.h>
#include <SDL\SDL.h>

#include "Engin\Engin.h"
#include "Engin\Renderer\Window.h"
#include "inih\INIReader.h"
#include "Engin\Core\Timer.h"

namespace Engin
{
	Engin::Engin() : 
		running(true), 
		accumulator(0.0f), 
		step(0.0f)
	{
		keyboardInput = new HID::KeyboardInput();
		mouseInput = new HID::MouseInput();
		gamepadInput = new HID::GamepadInput();
		eventManager = new Game::EventManager(keyboardInput, mouseInput, gamepadInput);
		myTimer.start();
	}

	Engin::~Engin()
	{
	}

	// Initialises the engine.
	void Engin::init(const std::string& path)
	{
		SDL_Init(SDL_INIT_EVERYTHING);

		// Open the config file and try to parse it.
		INIReader reader(path);

		if (reader.ParseError() < 0) 
		{
			std::cout << "Can't load " << path << std::endl;
		}
		else
		{
			std::cout << "Version: " << reader.GetReal("Engine", "version", -1) << std::endl;
		}

		// Set OpenGL attributes.
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
		
		// Create the window. Use data from the config.
		window.createWindow(reader.Get("Window", "title", "unnamed"),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			reader.GetInteger("Window", "width", 1280),
			reader.GetInteger("Window", "height", 720));

		SDL_SetWindowFullscreen(window.getSDLWindow(), reader.GetBoolean("Window", "fullscreen", false));

		step = 1.0f / (float)reader.GetReal("Physics", "step", 60.0f); // Read the physic step.

		glContext = SDL_GL_CreateContext(window.getSDLWindow()); // Create context.

		// Initialise glew and TTF.
		glewExperimental = GL_TRUE;
		glewInit();
		TTF_Init();

		glClearColor(0.2f, 0.4f, 0.6f, 1.0f); // Set clear color.

		// Enable GL stuff.
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Quits the engine.
	void Engin::quit()
	{
		delete eventManager;
		delete keyboardInput;
		delete mouseInput;
		SDL_GL_DeleteContext(glContext);

		TTF_Quit();
		SDL_Quit();
	}

	// Runs the engine.
	void Engin::run(Game::Scene* scene)
	{
		// Variables for deltatime
		float deltaTime = 0.0f;
		float newTime = 0.0f;
		float currentTime = myTimer.getLocalTime() / 10000.0f;

		sceneManager.change(scene); // Add scene to scene manager.
		sceneManager.handleScenes();

		// Start the main loop.
		while (running)
		{
			// Calculate delta time.
			newTime = myTimer.getLocalTime() / 10000.0f;
			deltaTime = std::min(newTime - currentTime, 0.25f);
			currentTime = newTime;

			// Handle events, update and draw.
			handleEvents();
			update(deltaTime);
			draw();

			// Updates current scene etc.
			sceneManager.handleScenes();
		}
	}

	// Handles the events.
	void Engin::handleEvents()
	{
		if (eventManager->userQuit())
		{
			running = false;
		}

		// eventManager must be updated last.
		eventManager->update();
	}

	// Updates the engine.
	void Engin::update(float deltaTime)
	{
		// Accumulator is used for fixed timestep. Using this we can
		// be certain that every update is of the same length. No need
		// to use delta time variable for game object movement etc.
		// Note that update can be called multiple times in one loop cycle.

		accumulator += deltaTime; // Add delta time to the accumulator.

		// Loop while accumulator is equal or larger than step.
		while (accumulator >= step)
		{
			// Update the scene manager and decrease accumulator by step.
			sceneManager.update(step);
			accumulator -= step;

			// Update inputs.
			keyboardInput->update();
			mouseInput->update();
			gamepadInput->update();
		}

		// Handle interpolation.
		// The above system works nice for physics alone but the graphics may lag behind. 
		// To fix this we are using interpolation. We use the time left in the
		// accumulator and divide it by step. Interpolation should be handled in scenes.
		sceneManager.interpolate(accumulator / step);
	}

	// Draws the engine.
	void Engin::draw()
	{
		// Clear color and depth buffers, draw scene manager and swap window.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneManager.draw();

		window.swapWindow();
	}
}