#pragma once

#include "Engin\Renderer\Window.h"
#include "Engin\Game\SceneManager.h"

namespace Engin
{
	class Engin
	{
	public:
		Engin();
		~Engin(); 

		void init(const std::string& path);
		void run(Game::Scene* scene);
		void quit();

		const Renderer::Window& getWindow() const { return window; }
		const Game::SceneManager& getSceneManager() const { return sceneManager; }
		const float getStep() const { return step; }
	private:
		void handleEvents();
		void update(float deltaTime);
		void draw();

		Renderer::Window window;
		Game::SceneManager sceneManager;
		SDL_GLContext glContext;
		bool running;
		float step;
		float accumulator;
	};
}