#include "Window.h"

namespace Engin
{
	namespace Renderer
	{
		void Window::createWindow(const std::string& title, int x, int y, int w, int h, Uint32 flags)
		{
			window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
		}
	}
}