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
			doge3 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/yellow_tile_40.png");
			std::cout << doge->getResourcePath() << ": " << doge->getHeight() << " " << doge->getReferenceCount() << std::endl;
		
			textString = "Press T to get visible tile count";
			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(18);
			
			textCreator = new Renderer::TextRenderer();
			textCreator->createTextTexture(font, textString, 0 ,0, 0);
			textTexture = textCreator->getTexture();
			k = 0;
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
				zoomByInput -= 0.03f;
			}
			if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_DOWN))
			{
				zoomByInput += 0.03f;
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

			
			
			//-----------------------------------------------------------------------------------------------------
			// Algorithm code
			//
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_RETURN))
			{
				point0 = glm::vec2(camera.getPositionRotationOrigin().x / 40.0f, camera.getPositionRotationOrigin().y / 40.0f);
				addVisiblePoint(point0);
						
				////Map bottom
				for (int i = 0; i < 20; i++)
				{
					point1 = glm::vec2(i, 0);
					octant = calculateOctant(point0, point1);

					temp0 = inputSwap(point0, octant);
					temp1 = inputSwap(point1, octant);

					plotLine(temp0, temp1);
				}
					
				//Map right side
				for (int i = 0; i < 20; i++)
				{
					point1 = glm::vec2(20, i);
					octant = calculateOctant(point0, point1);

					temp0 = inputSwap(point0, octant);
					temp1 = inputSwap(point1, octant);
					plotLine(temp0, temp1);
				}
				//Map top
				for (int i = 20; i > 0; i--)
				{
					point1 = glm::vec2(i, 20);
					octant = calculateOctant(point0, point1);

					temp0 = inputSwap(point0, octant);
					temp1 = inputSwap(point1, octant);
					plotLine(temp0, temp1);
				}
				//Map left side
				for (int i = 20; i > 0; i--)
				{
					point1 = glm::vec2(0, i);
					octant = calculateOctant(point0, point1);

					temp0 = inputSwap(point0, octant);
					temp1 = inputSwap(point1, octant);
					plotLine(temp0, temp1);
				}		
			}


			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_DELETE))
			{
				visibleTiles.clear();
				k = 0;
			}

			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_T))
			{
				/*textString = visibleTiles.size();				
				textCreator->createTextTexture(font, textString, 0, 0, 0);
				textTexture = textCreator->getTexture();*/
			}
		}

		void CameraTestScene::interpolate(GLfloat alpha)
		{
		}

		void CameraTestScene::draw()
		{
			textureBatch.begin();
			alphaTextureBatch.begin();
			
			alphaTextureBatch.draw(doge2, camera.getPositionRotationOrigin().x, camera.getPositionRotationOrigin().y, 1.0f, 0.1f);
			alphaTextureBatch.draw(textTexture, camera.getPositionRotationOrigin().x, camera.getPositionRotationOrigin().y+40.0f, 1.0f, 0.1f);

			if (visibleTiles.size() > 0)
			{
				for (int i = 0; i < visibleTiles.size(); i++)
				{
					alphaTextureBatch.draw(doge3, visibleTiles[i].x * 40, visibleTiles[i].y * 40, 1.0f, 0.0f);
				}
			}

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

		glm::vec2 CameraTestScene::inputSwap(glm::vec2 xy, int octant)
		{
			//input switch
			switch (octant)
			{
			case 0:
			{
				return xy;				
			}

			case 1:
			{
				return glm::vec2(xy.y, xy.x);
			}

			case 2:
			{
				return glm::vec2(xy.y, -xy.x);
			}

			case 3:
			{
				return glm::vec2(-xy.x, xy.y);
			}

			case 4:
			{
				return glm::vec2(-xy.x, -xy.y);
			}

			case 5:
			{
				return glm::vec2(-xy.y, -xy.x);
			}

			case 6:
			{
				return glm::vec2(-xy.y, xy.x);
			}

			case 7:
			{
				return glm::vec2(xy.x,-xy.y);
			}

			default:
				return glm::vec2(0);
			}
		}

		glm::vec2 CameraTestScene::outputSwap(glm::vec2 xy, int octant)
		{
			//input switch
			switch (octant)
			{
			case 0:
			{
				return xy;
			}

			case 1:
			{
				return glm::vec2(xy.y, xy.x);
			}

			case 2:
			{
				return glm::vec2(-xy.y, xy.x);
			}

			case 3:
			{
				return glm::vec2(-xy.x, xy.y);
			}

			case 4:
			{
				return glm::vec2(-xy.x, -xy.y);
			}

			case 5:
			{
				return glm::vec2(-xy.y, -xy.x);
			}

			case 6:
			{
				return glm::vec2(xy.y, -xy.x);
			}

			case 7:
			{
				return glm::vec2(xy.x, -xy.y);
			}

			default:
				return glm::vec2(0);
			}
		}

		void CameraTestScene::plotLine(glm::vec2 point0, glm::vec2 point1)
		{
			dx = point1.x - point0.x;
			dy = point1.y - point0.y;

			Difference = 2 * dy - dx;			

			y = point0.y;
			for (int x = point0.x + 1; x <= point1.x; x++)
			{
				temp = outputSwap(glm::vec2(x, y), octant);
				addVisiblePoint(glm::vec2(temp));

				Difference = Difference + 2 * dy;
				if (Difference > 0)
				{
					y++;
					Difference = Difference - (2 * dx);
				}
			}
		}

		int CameraTestScene::calculateOctant(glm::vec2 point0, glm::vec2 point1)
		{
			alpha = glm::atan(point1.y - point0.y, point1.x - point0.x);
			if (alpha < 0)
			{
				alpha = glm::radians(360.0f) + alpha;
			}

			if (alpha >= glm::radians(0.0f) && alpha <= glm::radians(45.0f))
			{
				return 0;
			}
			if (alpha >= glm::radians(45.0f) && alpha <= glm::radians(90.0f))
			{
				return 1;
			}
			if (alpha >= glm::radians(90.0f) && alpha <= glm::radians(135.0f))
			{
				return 2;
			}
			if (alpha >= glm::radians(135.0f) && alpha <= glm::radians(180.0f))
			{
				return 3;
			}
			if (alpha >= glm::radians(180.0f) && alpha <= glm::radians(225.0f))
			{
				return 4;
			}
			if (alpha >= glm::radians(225.0f) && alpha <= glm::radians(270.0f))
			{
				return 5;
			}
			if (alpha >= glm::radians(270.0f) && alpha <= glm::radians(315.0f))
			{
				return 6;
			}
			if (alpha >= glm::radians(315.0f) && alpha <= glm::radians(360.0f))
			{
				return 7;
			}
			return -1;
		}

		bool CameraTestScene::addVisiblePoint(glm::vec2 point)
		{
			int count = 0;
			
			if (visibleTiles.size() > 0)
			{
				for (int i = 0; i < visibleTiles.size(); i++)
				{
					if (visibleTiles[i] == point)
					{
						count++;
					}
				}
			}			

			if (count == 0)
			{
				visibleTiles.push_back(point);
			}

			return false;
		}
	}
}