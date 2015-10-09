#include "Engin\Renderer\Window.h"

namespace Engin
{
	namespace Renderer
	{
		void Window::createWindow(const std::string& title, int x, int y, int w, int h)
		{
			window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
			SDL_GLContext glcontext = SDL_GL_CreateContext(window);
		}
	}
}