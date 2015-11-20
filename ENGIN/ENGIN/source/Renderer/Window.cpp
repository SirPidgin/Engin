#include "Engin\Renderer\Window.h"

namespace Engin
{
	namespace Renderer
	{
		Window::Window() : window(nullptr)
		{
		}

		Window::~Window()
		{
			SDL_DestroyWindow(window);
		}

		void Window::createWindow(const std::string& title, int x, int y, int w, int h)
		{
			window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
			width = w;
			height = h;
		}

		void Window::swapWindow()
		{
			SDL_GL_SwapWindow(window);
		}
		
		const int Window::getWindowWidth() const
		{
			return width;
		}

		const int Window::getWindowHeight() const
		{
			return height;
		}
	}
}