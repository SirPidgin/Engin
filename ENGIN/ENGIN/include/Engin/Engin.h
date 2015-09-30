#pragma once

namespace Engin
{
	namespace Renderer
	{
		class Window;
	}

	void init();
	void initGlew();
	int update();
	void swapWindow(Renderer::Window& window);
	void quit();
}