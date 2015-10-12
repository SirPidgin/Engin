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



	Engin::initGlew();

	Engin::Renderer::Batch batch;

	while (Engin::update())
	{
		batch.drawTriangle(00.0f, 00.0f, 100.0f, 100.0f, 50.0f, 50.0f, Engin::Renderer::clrWhite, 0.0f);

		batch.flush(camera);
		batch.clear();
		window.swapWindow();
	}
	
	Engin::quit();

	return 0;
}