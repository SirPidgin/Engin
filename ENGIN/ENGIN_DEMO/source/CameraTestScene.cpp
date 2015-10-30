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
			camera1.setPositionCenter(3.0f*32.0f, 9.0f*32.0f);
			camera2.setPositionCenter(3.0f*32.0f, 9.0f*32.0f);

			//camera.setPosition(-100.0f, 0.0f);
			//camera.setPosition(100.0f, 0.0f);
			//camera.setPosition(800.0f, 0.0f);
			//camera.setPosition(96.0f, 96.0f);
			

			//camera2.setPosition(700.0f, 0.0f);			
			//camera2.setPosition(0.0f, 0.0f);

			//camera.setZoomLevel(4.0f);
			//camera.setZoomLevel(0.5f);

			shader.load("resources/shaders/vertex.shader", "resources/shaders/fragment.shader");
			textureShader.load("resources/shaders/texture_vertex.shader", "resources/shaders/texture_fragment.shader");
			batch.init(&shader, 4096);
			textureBatch.setShader(&textureShader);

			doge = resourceManager.load<Resources::Texture>("resources/doge.png");

			textureBatch.draw(doge, nullptr, 0.0f, 0.0f, 10.0f, 10.0f, 0.0f, 1.0f, Renderer::clrBlack, 1.0f, 1.0f);

			std::cout << doge->getResourcePath() << ": " << doge->getHeight() << " " << doge->getReferenceCount() << std::endl;
		
			
		}

		CameraTestScene::~CameraTestScene()
		{
			std::cout << "Camera scene shutdown" << std::endl;
		}

		void CameraTestScene::update(GLfloat step)
		{
			//camera.setRotation(this->alpha);
			camera2.setRotation(10*this->alpha);

			camera.setPosition(400 * glm::cos(alpha), 0.0f);

			camera1.setZoomLevel(1.0f + glm::cos(alpha));

			this->alpha += 0.005f;

			//std::cout << camera2.getPositionCenter().x << std::endl;
		}

		void CameraTestScene::interpolate(GLfloat alpha)
		{
		}

		void CameraTestScene::draw()
		{
			renderTilemap(0.0f, 0.0f, 32.0f, 32.0f, 25, 25, batch); //Depth not working?
			//renderTilemap(0.0f, 0.0f, 32.0f, 32.0f, 25, 25, batch);
			batch.drawTriangle(500.0f, 45.0f, 750.0f, 250.0f, 65.0f, 480.0f, Renderer::clrRed, 1.0f, 1.0f);
			batch.drawTriangle(10.0f, 10.0f, 100.0f, 10.0f, 50.0f, 50.0f, Renderer::clrGreen, 1.0f, 1.0f);
			batch.drawQuad(200.0f, 200.0f, 50.0f, 50.0f, Renderer::clrWhite, 1.0f, 1.0f);

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
						batch.drawQuad(startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Renderer::clrWhite, 0.5f, 0.0f);
					}
					else
					{
						batch.drawQuad(startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Renderer::clrBlack, 0.0f, 0.0f);
					}
				}
			}
		}
	}
}