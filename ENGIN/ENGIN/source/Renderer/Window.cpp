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
			SDL_GL_DeleteContext(glContext);
			SDL_DestroyWindow(window);

			glDeleteVertexArrays(1, &VAO);
		}

		void Window::createWindow(const std::string& title, int x, int y, int w, int h)
		{
			window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
			glContext =  SDL_GL_CreateContext(window);

			glewExperimental = GL_TRUE;
			glewInit();
			glClearColor(0.2f, 0.4f, 0.6f, 1.0f);

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);
		}

		void Window::swapWindow()
		{
			SDL_GL_SwapWindow(window);
		}
	}
}