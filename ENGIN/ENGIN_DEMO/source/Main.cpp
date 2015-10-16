#include <Engin\Engin.h>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Camera.h>
#include <Engin\Renderer\Batch.h>
#include <Engin\Renderer\Color.h>

namespace //camera test
{
	GLfloat alpha = 0.0f;
}

void renderTilemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight, Engin::Renderer::Batch& batch);

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

	Engin::Renderer::Batch batch(&shader, 4096);

	camera.setZoomLevel(0.9f);
	camera.setRotation(20.0f);

	camera2.setZoomLevel(2.0f);

	while (Engin::update())
	{
		batch.drawTriangle(500.0f, 45.0f, 750.0f, 250.0f, 65.0f, 480.0f, Engin::Renderer::clrRed, 0.0f);
		batch.drawTriangle(10.0f, 10.0f, 100.0f, 10.0f, 50.0f, 50.0f, Engin::Renderer::clrGreen, 0.0f);
		batch.drawQuad(200.0f, 200.0f, 50.0f, 50.0f, Engin::Renderer::clrWhite, 0.0f);
		renderTilemap(0.0f, 0.0f, 32.0f, 32.0f, 25, 25, batch);

		//camera test

		camera.setPosition(400.0f * cos(alpha), 0.0f);
		alpha += 0.01f;
		camera2.setZoomLevel(3.0f + 2.0f * cos(alpha));

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

void renderTilemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight, Engin::Renderer::Batch& batch)
{
	for (size_t y = 0; y < worldHeight; y++)
	{
		for (size_t x = 0; x < worldWidth; x++)
		{
			if ((x + y) % 2)
			{
				batch.drawQuad(startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Engin::Renderer::clrWhite, 0.0f);
			}
			else
			{
				batch.drawQuad(startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Engin::Renderer::clrBlack, 0.0f);
			}
		}
	}
}