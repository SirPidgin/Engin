#pragma once
#include <string>
#include <SDL\SDL.h>
#include <GL\glew.h>

namespace Engin
{
	namespace Renderer
	{
		class Window
		{
		public:
			Window();
			~Window();

			void createWindow(const std::string& title, int x, int y, int w, int h);
			void swapWindow();
			SDL_Window* getSDLWindow() { return window; }
		private:
			SDL_Window* window;
		};
	}
}