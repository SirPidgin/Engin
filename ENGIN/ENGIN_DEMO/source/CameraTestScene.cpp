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

			camera.initCamera(0.0f, 0.0f, 400.0f, 400.0f, 0.0f, 0.0f, 1.0f);
			camera1.initCamera(0.0f, 400.0f, 400.0f, 320.0f, 0.0f, 0.0f, 1.0f);
			camera2.initCamera(400.0f, 0.0f, 880.0f, 720.0f, 0.0f, 0.0f, 1.0f);
			
			//camera.setRotation(45);
			camera.setZoomLevel(0.5f);			
			//camera.setZoomLevel(0.01f);	
			//camera.setZoomLevel(0.8f);
			//camera.setZoomLevel(0.6f);

			camera.setPositionCenter(3.0f*32.0f, 9.0f*32.0f);

			//camera.setPosition(-100.0f, 0.0f);
			//camera.setPosition(100.0f, 0.0f);
			//camera.setPosition(800.0f, 0.0f);
			//camera.setPosition(96.0f, 96.0f);
			

			//camera2.setPosition(700.0f, 0.0f);
			camera2.setPositionCenter(3.0f*32.0f, 9.0f*32.0f);
			//camera2.setPosition(0.0f, 0.0f);

			//camera.setZoomLevel(4.0f);
			//camera.setZoomLevel(0.5f);

			shader.load("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");
			batch.addShader(&shader, 4096);
		}

		CameraTestScene::~CameraTestScene()
		{
			std::cout << "Camera scene shutdown" << std::endl;
		}

		void CameraTestScene::update(GLfloat deltaTime)
		{
			renderTilemap(0.0f, 0.0f, 32.0f, 32.0f, 25, 25, batch); //Depth not working?
			batch.drawTriangle(500.0f, 45.0f, 750.0f, 250.0f, 65.0f, 480.0f, Renderer::clrRed, 0.0f);
			batch.drawTriangle(10.0f, 10.0f, 100.0f, 10.0f, 50.0f, 50.0f, Renderer::clrGreen, 0.0f);
			batch.drawQuad(200.0f, 200.0f, 50.0f, 50.0f, Renderer::clrWhite, 0.0f);			
			//renderTilemap(0.0f, 0.0f, 32.0f, 32.0f, 25, 25, batch);

			camera.setRotation(alpha);
			camera2.setRotation(alpha);

			alpha += 0.5f;

			camera.fixCoordinatesForRotationAtTheEndOfUpdate();
			camera2.fixCoordinatesForRotationAtTheEndOfUpdate();

			//std::cout << camera2.getPositionCenter().x << std::endl;
		}

		void CameraTestScene::draw()
		{
			camera.activateViewPort();
			batch.flush(camera);

			camera1.activateViewPort();
			batch.flush(camera1);

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