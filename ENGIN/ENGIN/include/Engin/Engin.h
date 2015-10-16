#pragma once

#include "Engin\Renderer\Window.h"

namespace Engin
{
	class Engin
	{
	public:
		Engin();
		~Engin(); 

		void init(const std::string& path);
		int update();
		void quit();

		const Renderer::Window& getWindow() const { return window; }
	private:
		Renderer::Window window;
		SDL_GLContext glContext;
		GLuint VAO;
	};
}