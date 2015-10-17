#include "CameraTestScene.h"

#include <iostream>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Color.h>

namespace Engin
{
	namespace Game
	{
		CameraTestScene::CameraTestScene()
		{
			alpha = 0.0f;

			std::cout << "Camera test scene going on, be aware of rotating cameras" << std::endl;

			camera.initCamera(0.0f, 0.0f, 400.0f, 720.0f, 0.0f, 0.0f, 1.0f);
			camera2.initCamera(400.0f, 0.0f, 880.0f, 720.0f, 300.0f, 0.0f, 1.0f);

			camera.setZoomLevel(0.5f);
			//camera.setRotation(20.0f);

			shader.load("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");
			batch.addShader(&shader, 4096);
		}

		CameraTestScene::~CameraTestScene()
		{
			std::cout << "Camera scene shutdown" << std::endl;
		}

		void CameraTestScene::update(GLfloat deltaTime)
		{
			batch.drawTriangle(500.0f, 45.0f, 750.0f, 250.0f, 65.0f, 480.0f, Renderer::clrRed, 0.0f);
			batch.drawTriangle(10.0f, 10.0f, 100.0f, 10.0f, 50.0f, 50.0f, Renderer::clrGreen, 0.0f);
			batch.drawQuad(200.0f, 200.0f, 50.0f, 50.0f, Renderer::clrWhite, 0.0f);
			renderTilemap(0.0f, 0.0f, 32.0f, 32.0f, 25, 25, batch);

			//camera test
			//camera2.setZoomLevel(4.0f - cos(alpha)*2); //Zoom position could be relative to camera center if even possible.
			camera2.setPosition(700.0f, 0.0f);
			camera2.setRotation(alpha * 30);

			camera.setPositionCenter(200.0f + 200.0f * sin(alpha), 0.0f);
			alpha += 0.01f;
		}

		void CameraTestScene::draw()
		{
			camera.activateViewPort();
			batch.flush(camera);

			camera2.activateViewPort();
			batch.flush(camera2);

			batch.clear();
		}

		void CameraTestScene::renderTilemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight, Renderer::Batch& batch)
		{
			for (size_t y = 0; y < worldHeight; y++)
			{
				for (size_t x = 0; x < worldWidth; x++)
				{
					if ((x + y) % 2)
					{
						batch.drawQuad(startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Renderer::clrWhite, 0.0f);
					}
					else
					{
						batch.drawQuad(startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Renderer::clrBlack, 0.0f);
					}
				}
			}
		}
	}
}