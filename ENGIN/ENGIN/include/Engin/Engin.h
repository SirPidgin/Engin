#pragma once

#include "Engin\Renderer\Window.h"
#include "Engin\Game\SceneManager.h"
#include "Engin\Game\EventManager.h"
#include "Engin\HID\KeyboardInput.h"
#include "Engin\HID\MouseInput.h"
#include "Core\Timer.h"s

namespace Engin
{
	// Engin is the main class of the Engin engine. It handles the 
	// initialisation, updating and cleaning of the engine.

	class Engin
	{
	public:
		Engin();
		~Engin(); 

		void init(const std::string& path);
		void run(Game::Scene* scene);
		void stop() { running = false; }
		void quit();

		const Renderer::Window& getWindow() const 
		{ 
			return window; 
		}

		Game::SceneManager& getSceneManager() 
		{ 
			return sceneManager; 
		}

		const float getStep() const 
		{ 
			return step; 
		}

		HID::KeyboardInput* keyboardInput;
		HID::MouseInput* mouseInput;
		HID::GamepadInput* gamepadInput;
	private:
		void handleEvents();
		void update(float deltaTime);
		void draw();

		Renderer::Window window;
		Game::SceneManager sceneManager;
		Game::EventManager* eventManager;

		SDL_GLContext glContext;
		bool running;
		float step;
		float accumulator;
		Core::Timer myTimer;
	};
}