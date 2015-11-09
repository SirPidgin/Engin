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
		CameraTestScene::CameraTestScene(Engin* engine)
		{
			this->engine = engine;

			std::cout << "Camera test scene going on, be aware of rotating cameras" << std::endl;

			camera.initCamera(0.0f, 0.0f, 800.0f, 800.0f, 0.0f, 0.0f, 400, 400);
			
			shader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/shader");
			textureShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/texture_shader");
			alphaShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/alpha_shader");
			batch.init(shader, 4096);
			textureBatch.setShader(textureShader);
			alphaTextureBatch.setShader(alphaShader);
			alphaTextureBatch.setSortMode(Renderer::TextureSortMode::FrontToBack);

			doge = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/white_tile_40.png");
			doge2 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_40.png");
			std::cout << doge->getResourcePath() << ": " << doge->getHeight() << " " << doge->getReferenceCount() << std::endl;
		
			textString = "Press T to get Global time";
			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(100);
			
			textCreator = new Renderer::TextRenderer();
			textCreator->createTextTexture(font, textString, 255 ,255, 255);
			textTexture = textCreator->getTexture();
		}

		CameraTestScene::~CameraTestScene()
		{
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
			camera.setZoomLevel(zoomByInput); //by input

			static float moveByInputX = 400.0f;
			static float moveByInputY = 400.0f;
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_W))
			{
				//std::cout << "W" << std::endl;
				moveByInputY += 40.0f;
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_A))
			{
				//std::cout << "A" << std::endl;
				moveByInputX -= 40.0f;
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_S))
			{
				//std::cout << "S" << std::endl;
				moveByInputY -= 40.0f;
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_D))
			{
				//std::cout << "D" << std::endl;
				moveByInputX += 40.0f;
			}
			camera.setPositionRotationOrigin(moveByInputX, moveByInputY); //by input
			
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_SPACE))
			{
				std::cout << camera.getPositionRotationOrigin().x << " - " << camera.getPositionRotationOrigin().y << std::endl;
			}

			static float rotateByInput = 0.0f;
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_LEFT))
			{
				//std::cout << "<-" << std::endl;
				rotateByInput += 10.0f;
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_RIGHT))
			{
				//std::cout << "->" << std::endl;
				rotateByInput -= 10.0f;
			}
			camera.setRotation(rotateByInput); //by input
	
		}

		void CameraTestScene::interpolate(GLfloat alpha)
		{
		}

		void CameraTestScene::draw()
		{
			textureBatch.begin();
			alphaTextureBatch.begin();
			//batch.drawQuad(0.0f, 0.0f, 64.0f, 64.0f, Renderer::clrWhite, 1.0f, 1.0f);
			
			alphaTextureBatch.draw(doge2, camera.getPositionRotationOrigin().x, camera.getPositionRotationOrigin().y, 1.0f, 0.1f);
			
			renderDogemap(0.0f, 0.0f, 40.0f, 40.0f, 21, 21);

			textureBatch.end();
			alphaTextureBatch.end();

			camera.activateViewPort();
			batch.flush(camera);
			textureBatch.flush(camera);
			alphaTextureBatch.flush(camera);

			textureBatch.clear();
			alphaTextureBatch.clear();
			batch.clear();
		}

		void CameraTestScene::renderDogemap(float startX, float startY, float tileWidth, float tileHeight, size_t worldWidth, size_t worldHeight)
		{
			for (size_t y = 0; y < worldHeight; y++)
			{
				for (size_t x = 0; x < worldWidth; x++)
				{
					if ((x + y) % 2)
					{
						alphaTextureBatch.draw(doge, startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Renderer::clrWhite);
					}
					else
					{
						alphaTextureBatch.draw(doge, startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, {(0.4f),(0.4f),(0.4f)});
					}
				}
			}
		}
	}
}