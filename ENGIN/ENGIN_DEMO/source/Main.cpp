#include <Engin\Engin.h>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>
#include <Engin\Renderer\Color.h>

int main(int argc, char** argv)
{
	Engin::init();
	Engin::Renderer::Window window;
	Engin::Renderer::Camera camera;
	Engin::Renderer::Camera camera2;

	window.createWindow("HAHAHAHA", 300.0f, 300.0f, 800.0f, 600.0f);
	camera.initCamera(0.0f, 0.0f, 400.0f, 600.0f, 0.0f, 0.0f, 1.0f);
	camera2.initCamera(400.0f, 0.0f, 400.0f, 600.0f, 0.0f, 0.0f, 1.0f);

	Engin::Resources::Shader shader("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");

	Engin::Renderer::Batch batch(&shader);

	while (Engin::update())
	{
		batch.drawTriangle(500.0f, 45.0f, 750.0f, 250.0f, 65.0f, 480.0f, Engin::Renderer::clrRed, 0.0f);
		batch.drawTriangle(10.0f, 10.0f, 100.0f, 10.0f, 50.0f, 50.0f, Engin::Renderer::clrGreen, 0.0f);

		camera.activateViewPort();
		batch.flush(camera);

		camera2.activateViewPort();
		batch.flush(camera2);

		batch.clear();

		window.swapWindow();
	}
	
	Engin::quit();

	return 0;
}