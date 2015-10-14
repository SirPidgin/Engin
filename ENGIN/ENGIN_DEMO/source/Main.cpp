#include <Engin\Engin.h>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>
#include <Engin\Renderer\Color.h>
#include <iostream>

int main(int argc, char** argv)
{
	Engin::init();
	Engin::Renderer::Window window;
	Engin::Renderer::Camera camera;

	window.createWindow("HAHAHAHA", 300, 300, 800, 600);
	camera.initCamera(0, 0, 800, 600, 0, 0, 1);

	Engin::Resources::Shader shader("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");

	Engin::Renderer::Batch batch(&shader);

	while (Engin::update())
	{
		batch.drawTriangle(500.0f, 45.0f, 750.0f, 250.0f, 65.0f, 480.0f, Engin::Renderer::clrRed, 0.0f);
		batch.drawTriangle(10.0f, 10.0f, 100.0f, 10.0f, 50.0f, 50.0f, Engin::Renderer::clrGreen, 0.0f);

		batch.flush(camera);
		batch.clear();
		window.swapWindow();
	}
	
	Engin::quit();

	return 0;
}