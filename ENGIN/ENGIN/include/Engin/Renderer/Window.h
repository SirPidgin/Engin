#pragma once
#include <string>
#include <SDL\SDL.h>

namespace Engin
{
	namespace Renderer
	{
		class Window
		{
		public:
			Window() : window(nullptr) {}
			~Window() {}

			void createWindow(const std::string& title, int x, int y, int w, int h);
		private:
			SDL_Window* window;
		};
	}
}