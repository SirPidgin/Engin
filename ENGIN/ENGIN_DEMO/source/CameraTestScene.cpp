#include "CameraTestScene.h"

#include <iostream>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Color.h>
#include <glm\gtc\type_ptr.hpp>
#include "irrKlang\irrKlang.h"

irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

namespace Engin
{
	namespace Game
	{
		CameraTestScene::CameraTestScene(Engin* engine) : camera(createWorldCamera()), camera1(createWorldCamera()), camera2(createWorldCamera())
		{
			this->engine = engine;
			alpha = 0.0f;

			//camera = createGuiCamera();

			std::cout << "Camera test scene going on, be aware of rotating cameras" << std::endl;

			
			camera->initCamera(0.0f, 0.0f, 400.0f, 400.0f, 0.0f, 0.0f, 200, 200);
			camera1->initCamera(0.0f, 400.0f, 400.0f, 320.0f, 0.0f, 0.0f, 200, 160);
			camera2->initCamera(400.0f, 0.0f, 880.0f, 720.0f, 0.0f, 0.0f, 440, 144);

			//camera->setRotation(45);
			camera->setZoomLevel(0.5f);
			//camera->setZoomLevel(0.01f);	
			//camera->setZoomLevel(0.8f);
			//camera->setZoomLevel(0.6f);

			camera->setPositionRotationOrigin(3.0f*32.0f, 9.0f*32.0f);
			camera1->setPositionRotationOrigin(3.0f*32.0f, 9.0f*32.0f);
			camera2->setPositionRotationOrigin(5 * 64.0f, 9 * 64.0f);

			//camera->setZoomLevel(4.0f);
			//camera->setZoomLevel(0.5f);
			shader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/shader");
			textureShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/texture_shader");
			alphaShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/alpha_shader");

			opaqueBatch.setShader(textureShader);
			alphaBatch.setShader(alphaShader);
			alphaBatch.setSortMode(Renderer::TextureSortMode::FrontToBack);

			doge = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/doge.png");
			//SoundEngine->play2D("resources/rossini.mp3", GL_TRUE);
		
			textString = "Press T to get Global time";
			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(100);

			animation = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/test.xml");
			animation1 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/testmushroom.xml");

			animationPlayer.setAnimation(animation);
			animationPlayer.loopable(true);
			animationPlayer.start();
			
			animationPlayer1.setAnimation(animation1);
			animationPlayer1.loopable(true);
			animationPlayer1.start();
			
			textCreator = new Renderer::TextRenderer();
			textCreator->createTextTexture(font, textString, 255 ,255, 255);
			textTexture = textCreator->getTexture();

			Resources::ResourceManager::getInstance().printResources();
		}

		CameraTestScene::~CameraTestScene()
		{
			// Unload resources.
			Resources::ResourceManager::getInstance().unload(shader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(textureShader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(alphaShader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(doge->getResourcePath());
			Resources::ResourceManager::getInstance().unload(font->getResourcePath());
			Resources::ResourceManager::getInstance().unload(animation->getResourcePath());
			Resources::ResourceManager::getInstance().unload(animation1->getResourcePath());

			std::cout << "Camera scene shutdown" << std::endl;
		}

		void CameraTestScene::update(GLfloat step)
		{
			static float zoomByInput = 1.0f;
			if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_UP))
			{
				if (zoomByInput > 0.0f)
				zoomByInput -= glm::radians(2.0f); //???? Eikös glm::radians(2.0f) ole aina 0.034 ja risat
			}
			if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_DOWN))
			{
				zoomByInput += glm::radians(2.0f);
			}
			camera2->setZoomLevel(zoomByInput); //by input
			
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_P))
			{
				animationPlayer.pause();
			}

			static float moveByInputX = 0.0f;
			static float moveByInputY = 0.0f;
			static float moveSpeed = 64.0f;
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_W))
			{
				//std::cout << "W" << std::endl;
				moveByInputY += moveSpeed;
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_A))
			{
				//std::cout << "A" << std::endl;
				moveByInputX -= moveSpeed;
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_S))
			{
				//std::cout << "S" << std::endl;
				moveByInputY -= moveSpeed;
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_D))
			{
				//std::cout << "D" << std::endl;
				moveByInputX += moveSpeed;
			}
			camera2->setPositionRotationOrigin(moveByInputX, moveByInputY); //by input
			
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_SPACE))
			{
				std::cout << camera2->getPositionRotationOrigin().x << " - " << camera2->getPositionRotationOrigin().y << std::endl;
			}

			static float rotateByInput = 0.0f;
			static float rotateSpeed = 0.01f;
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_LEFT))
			{
				//std::cout << "<-" << std::endl;
				rotateByInput += rotateSpeed;
			}
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_RIGHT))
			{
				//std::cout << "->" << std::endl;
				rotateByInput -= rotateSpeed;
			}
			camera2->setRotation(rotateByInput); //by input

			//camera.setRotation(this->alpha);
			//camera2.setRotation(30 * alpha);
			camera->setPositionRotationOrigin(400 * glm::cos(alpha), 0.0f);
			camera1->setZoomLevel(1.0f + glm::cos(alpha));
			this->alpha += 0.005f;

			animationPlayer.update();
			animationPlayer1.update();

			//Test:
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_T))
			{
				GLfloat time = Core::Timer::getGlobalTime()/1000.0f;
				textString = "Global time: " + std::to_string(time) + " seconds";
				textCreator->createTextTexture(font, textString, 255, 255, 255);
				textTexture = textCreator->getTexture();
			}

			// Back to menu.
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_ESCAPE))
			{
				engine->getSceneManager().pop();
			}
		}

		void CameraTestScene::interpolate(GLfloat alpha)
		{
		}

		void CameraTestScene::draw()
		{
			alphaBatch.draw(doge, nullptr, 3*cos(alpha) * 110.0f, 3*sin(alpha) * 110.0f, 250.0f, 250.0f, alpha * 1000.0f, 1.0f, Renderer::clrWhite, glm::abs(glm::sin(alpha)), 0.9f);
			alphaBatch.draw(doge, nullptr, 0.0f, 0.0f, 100.0f, 100.0f, 0.0f, 1.0f, Renderer::clrRed, 0.5f, 1.0f);
			alphaBatch.draw(doge, &glm::vec4(0.0f, 0.0f, 300.0f, 300.0f), 350.0f, 350.0f, 125.0f, 250.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 1.0f);
			alphaBatch.draw(doge, nullptr, 600.0f, 600.0f, 50.0f, 50.0f, 0.0f, 1.0f, Renderer::clrGreen, 0.5f, 0.1f);
			alphaBatch.draw(doge, nullptr, 610.0f, 610.0f, 50.0f, 50.0f, 0.0f, 1.0f, Renderer::clrWhite, 0.3f, 0.2f);
			alphaBatch.draw(doge, nullptr, 620.0f, 620.0f, 50.0f, 50.0f, 0.0f, 1.0f, Renderer::clrRed, 0.4f, 0.3f);
			alphaBatch.draw(doge, nullptr, 630.0f, 600.0f, 50.0f, 50.0f, 0.0f, 1.0f, Renderer::clrBlue, 0.7f, 0.4f);
			alphaBatch.draw(doge, 500.0f, 0.0f, 0.5f, 0.1f);
			alphaBatch.draw(textTexture, 0.0f, 0.0f, 0.8f, 0.5f); //coordinates not working correctly for text
			alphaBatch.draw(animationPlayer.getTexture(), animationPlayer.getCurrentFrameTexCoords(), 
				camera2->getPositionRotationOrigin().x, camera2->getPositionRotationOrigin().y, 256.0f, 256.0f, 
				128.0f, 128.0f, 
				camera2->getRotation(), 1.0f, Renderer::clrGreen, 1.0f, 0.7f);
			alphaBatch.draw(animationPlayer1.getTexture(), animationPlayer1.getCurrentFrameTexCoords(), 
				0.0f, 0.0f, 512.0f, 512.0f, 
				256.0f, 256.0f,
				0.0f, 2.0f, Renderer::clrRed, 1.0f, 1.0f);
			renderDogemap(0.0f, 0.0f, 64.0f, 64.0f, 20, 20);
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

		void CameraTestScene::renderDogemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight)
		{
			for (size_t y = 0; y < worldHeight; y++)
			{
				for (size_t x = 0; x < worldWidth; x++)
				{
					if ((x + y) % 2)
					{
						alphaBatch.draw(doge, startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Renderer::clrRed);
					}
					else
					{
						alphaBatch.draw(doge, startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Renderer::clrBlue);
					}
				}
			}
		}
	}
}