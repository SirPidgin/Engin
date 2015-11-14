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
#pragma region INIT
			this->engine = engine;

			std::cout << "Camera test scene going on, be aware of rotating cameras" << std::endl;

			camera.initCamera(0.0f, 0.0f, 800.0f, 800.0f, 0.0f, 0.0f, 400, 400);
			camera2.initCamera(0.0f, 700.0f, 800.0f, 200.0f, 0.0f, 0.0f, 400, 50);

			shader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/shader");
			textureShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/texture_shader");
			alphaShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/alpha_shader");
			batch.init(shader, 2*4096);
			textureBatch.setShader(textureShader);
			alphaTextureBatch.setShader(alphaShader);
			alphaTextureBatch.setSortMode(Renderer::TextureSortMode::FrontToBack);
			alphaTextureBatch2.setShader(alphaShader);
			alphaTextureBatch2.setSortMode(Renderer::TextureSortMode::FrontToBack);

			doge = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/white_tile_40.png");
			doge4 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_40.png");
			doge2 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/yellow_tile_40.png");
			doge1 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_tile_40.png");
			doge3 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/green_tile_40.png");
			doge5 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/red_tile_40.png");

			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(40);
			textCreator.createTextTexture(font, "|", 255,100,0);
			text = textCreator.getTexture();
			textCreator3.createTextTexture(font, "Visible tile count: ", 255, 100, 0);
			text3 = textCreator3.getTexture();

			mapX = 40;
			mapY = 60;
			tileSize = 40;

			visibleTiles.resize((mapX)*(mapY));
			objectTiles.resize((mapX + 1)*(mapY + 1));
			
			emptyVector(2);
			emptyVector(3);		

			alpha = 0.0f;
#pragma endregion

#pragma region WallTiles
			
			//MapSides
			for (int i = 0; i <= mapY; i++)
			{
				addIntoVector(1, glm::vec2(0, i), 1);
			}

			for (int i = 0; i <= mapY; i++)
			{
				addIntoVector(1, glm::vec2(mapX, i), 1);
			}

			for (int i = 0; i <= mapX; i++)
			{
				addIntoVector(1, glm::vec2(i, mapY), 1);
			}

			for (int i = 0; i <= mapX; i++)
			{
				addIntoVector(1, glm::vec2(i, 0), 1);
			}			
			//---------------------

			addIntoVector(1, glm::vec2(2, 19), 1);

			for (int i = 10; i < 15; i++)
			{
				addIntoVector(1, glm::vec2(i, 18), 1);
			}

			for (int i = 13; i < 18; i++)
			{
				addIntoVector(1, glm::vec2(2, i), 1);
			}

			for (int i = 11; i < 16; i++)
			{
				addIntoVector(1, glm::vec2(0, i), 1);
			}

			for (int i = 1; i < 11; i++)
			{
				addIntoVector(1, glm::vec2(i, 11), 1);
			}

			for (int i = 2; i < 11; i++)
			{
				addIntoVector(1, glm::vec2(i, 9), 1);
			}

			for (int i = 15; i < 19; i++)
			{
				addIntoVector(1, glm::vec2(i, 3), 1);
			}

			for (int i = 4; i < 7; i++)
			{
				addIntoVector(1, glm::vec2(18, i), 1);
			}

			for (int i = 8; i < 17; i++)
			{
				addIntoVector(1, glm::vec2(15, i), 1);
			}

			for (int i = 12; i < 16; i++)
			{
				addIntoVector(1, glm::vec2(i, 7), 1);
			}

			for (int i = 2; i < 13; i++)
			{
				addIntoVector(1, glm::vec2(i, 3), 1);
			}

			for (int i = 20; i < 40; i++)
			{
				addIntoVector(1, glm::vec2(i, 20), 1);
			}
#pragma endregion
			std::cout << ""<< std::endl;
		}

		CameraTestScene::~CameraTestScene()
		{
			std::cout << "Camera scene shutdown" << std::endl;
		}

		void CameraTestScene::update(GLfloat step)
		{
#pragma region KeyboardMovement
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

			static int layerX = 10;
			static int layerY = 10;
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_W))
			{
				//std::cout << "W" << std::endl;
				if (layerY < mapY*tileSize)
				{
					layerY += 1;
				}
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_A))
			{
				//std::cout << "A" << std::endl;
				if (layerX > 0)
				{
					layerX -= 1;
				}
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_S))
			{
				//std::cout << "S" << std::endl;
				if (layerY > 0)
				{
					layerY -= 1;
				}
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_D))
			{
				//std::cout << "D" << std::endl;
				if (layerX < mapX*tileSize)
				{
					layerX += 1;
				}
			}

			playerX = layerX;
			playerY = layerY;
			camera.setPositionRotationOrigin(playerX*tileSize, playerY*tileSize);

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

#pragma endregion

			

			myTimer.start();
			//During Update
			emptyVector(2);
			emptyVector(3);

			friendX = 17;
			friendY = 10;
			addIntoVector(1, glm::vec2(friendX, friendY), 3);

			calculateVision(playerX, playerY);
			calculate90(playerX, playerY);			
			myTimer.pause();

			//Information
			textCreator.createTextTexture(font, "Vision calculation time: " + std::to_string(myTimer.getLocalTime()) + " ms", 255, 100, 0);
			text = textCreator.getTexture();
			textCreator3.createTextTexture(font, "Visible tiles count: " + std::to_string(visibleTilesCount), 255, 100, 0);
			text3 = textCreator3.getTexture();

			alpha += 0.01f;
		}

		void CameraTestScene::interpolate(GLfloat alpha)
		{
		}

		void CameraTestScene::draw()
		{
			//myTimer2.start();
			textureBatch.begin();
			alphaTextureBatch.begin();
			alphaTextureBatch2.begin();
			
			//player
			alphaTextureBatch.draw(doge4, playerX*tileSize, playerY*tileSize, 1.0f, 0.9f);
			
			//always drawn: friend
			alphaTextureBatch.draw(doge4, friendX*tileSize, friendY * tileSize, 1.0f, 0.5f);

			//drawing data vectors
			for (int i = 0; i <= mapY; i++)
			{
				for (int j = 0; j <= mapX; j++)
				{
					if (objectTiles[i*mapX + j] == 1) //Wall tiles
					{
						textureBatch.draw(doge1, j * tileSize, i * tileSize, 1.0f, 0.4f);
					}
				}				
			}

			for (int i = 0; i < mapY; i++)
			{
				for (int j = 0; j < mapX; j++)
				{
					if (visibleTiles[i * mapX + j] == 1) //Visible
					{
						alphaTextureBatch.draw(doge2, j * tileSize, i * tileSize, 0.5f, 0.6f);
					}
					if (visibleTiles[i * mapX + j] == 5) //Wall was hit
					{
						alphaTextureBatch.draw(doge5, j * tileSize, i * tileSize, 0.5f, 0.6f);
					}
					if (visibleTiles[i * mapX + j] == 3) //friend
					{
						alphaTextureBatch.draw(doge3, j * tileSize, i * tileSize, 0.5f, 0.6f);
					}
				}
			}
			//----------------
			
			alphaTextureBatch2.draw(text, &glm::vec4(0.0f, 0.0f, text->getWidth(), text->getHeight()), camera2.getPositionRotationOrigin().x + glm::cos(alpha)*400.0f, camera2.getPositionRotationOrigin().y + 25, text->getWidth(), text->getHeight(), 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 0.9f);
			alphaTextureBatch2.draw(text3, camera2.getPositionRotationOrigin().x, camera2.getPositionRotationOrigin().y - 30, 1.0f, 0.9f);

			renderDogemap(0.0f, 0.0f, tileSize, tileSize, mapX + 1, mapY + 1);

			textureBatch.end();
			alphaTextureBatch.end();
			alphaTextureBatch2.end();
			//Timer
			
			
			

			camera.activateViewPort();
			batch.flush(camera);
			textureBatch.flush(camera);
			alphaTextureBatch.flush(camera);

			camera2.activateViewPort();
			alphaTextureBatch2.flush(camera2);

			textureBatch.clear();
			alphaTextureBatch.clear();
			alphaTextureBatch2.clear();
			batch.clear();

			//myTimer2.pause();
		}

		void CameraTestScene::renderDogemap(float startX, float startY, int tileWidth, int tileHeight, size_t worldWidth, size_t worldHeight)
		{
			for (size_t y = 0; y < worldHeight; y++)
			{
				for (size_t x = 0; x < worldWidth; x++)
				{
					if ((x + y) % 2)
					{
						textureBatch.draw(doge, startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, Renderer::clrWhite);
					}
					else
					{
						textureBatch.draw(doge, startX + x * tileWidth, startY + y * tileHeight, tileWidth, tileHeight, {(0.4f),(0.4f),(0.4f)});
					}
				}
			}
		}
		
		//-----------------------------------------------------------------------------------------------------
		// Algorithm code
		//
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

			if (checkIfVisiblePoint(this->point0) == true)
			{
				addIntoVector(2, temp, 5); //Mark the hit wall
				return; //Wall was hit
			}
			y = point0.y;
			for (int x = point0.x+1; x <= point1.x; x++)
			{				
				if (Difference > 0)
				{
					y++;
					Difference = Difference - (2 * dx);
				}
				
				//plot
				temp = outputSwap(glm::vec2(x, y), octant);
				if (checkIfVisiblePoint(glm::vec2(temp)) == true)
				{
					addIntoVector(2, temp, 5); //Mark the hit wall
					return; //Wall was hit
				}

				Difference = Difference + 2 * dy;
			}
		}

		int CameraTestScene::calculateOctant(glm::vec2 point0, glm::vec2 point1)
		{
			m = (point1.y - point0.y) / (point1.x - point0.x);

			if (glm::isinf(m))
			{
				return 90;
			}

			if (m >= 0.0f && m <= 1.0f && point0.x < point1.x)
			{
				return 0;
			}

			if (m > 1.0f && m < DBL_MAX && point0.y < point1.y)
			{
				return 1;
			}

			if (m < -1.0f && m > -DBL_MAX && point0.y < point1.y)
			{
				return 2;
			}

			if (m <= 0.0f && m >= -1.0f && point1.x < point0.x)
			{
				return 3;
			}

			if (m > 0.0f && m <= 1.0f && point1.x < point0.x)
			{
				return 4;
			}

			if (m > 1.0f && m < DBL_MAX && point1.y < point0.y)
			{
				return 5;
			}

			if (m < -1.0f && m > -DBL_MAX && point1.y < point0.y)
			{
				return 6;
			}

			if (m <= 0.0f && m >= -1.0f && point0.x < point1.x)
			{
				return 7;
			}

		}

		bool CameraTestScene::checkIfVisiblePoint(glm::vec2 point)
		{
			int count = 0;

			if (objectTiles[point.y * mapX + point.x] == 1)
			{
				return true; //wall was hit
			}

			if (visibleTiles[point.y * mapX + point.x] == 0)
			{
				addIntoVector(2, point, 1); //we see the tile
				visibleTilesCount++;
				count++;
			}

			if (count > 0)
			{
				if (objectTiles[point.y * mapX + point.x] == 3)
				{
					addIntoVector(2, point, 3); //we see a friend in the tile as well
				}
			}

			return false; //There was no wall
		}
		
		void CameraTestScene::calculateVision(int playerX,int playerY) //Limits for vision
		{
			visibleTilesCount = 0;
			//point0 = glm::vec2(playerX, playerY);
			
#pragma region DrawLineToCorner
			//point1 = glm::vec2(mapX-1, mapY-1);
			//octant = calculateOctant(point0, point1);
			//if (octant != 90)
			//{
			//	temp0 = inputSwap(point0, octant);
			//	temp1 = inputSwap(point1, octant);

			//	plotLine(temp0, temp1);
			//}

			//point1 = glm::vec2(1, mapY-1);
			//octant = calculateOctant(point0, point1);
			//if (octant != 90)
			//{
			//	temp0 = inputSwap(point0, octant);
			//	temp1 = inputSwap(point1, octant);

			//	plotLine(temp0, temp1);
			//}

			//point1 = glm::vec2(mapX-1, 1);
			//octant = calculateOctant(point0, point1);
			//if (octant != 90)
			//{
			//	temp0 = inputSwap(point0, octant);
			//	temp1 = inputSwap(point1, octant);

			//	plotLine(temp0, temp1);
			//}

			//point1 = glm::vec2(1, 1);
			//octant = calculateOctant(point0, point1);
			//if (octant != 90)
			//{
			//	temp0 = inputSwap(point0, octant);
			//	temp1 = inputSwap(point1, octant);

			//	plotLine(temp0, temp1);
			//}  
#pragma endregion

			for (int i = 0; i <= 7; i++)
			{
				calculateVisionOctant(playerX, playerY, i);
			}
			
#pragma region Old
			//Map bottom
			//for (int i = 1; i < mapX; i++)
			//{
			//	point1 = glm::vec2(i, 1);
			//	octant = calculateOctant(point0, point1);
			//	if (octant != 90)
			//	{
			//		temp0 = inputSwap(point0, octant);
			//		temp1 = inputSwap(point1, octant);

			//		plotLine(temp0, temp1);
			//	}				
			//}

			////Map right side
			//for (int i = 1; i < mapY; i++)
			//{
			//	point1 = glm::vec2(mapX-1, i);
			//	octant = calculateOctant(point0, point1);
			//	if (octant != 90)
			//	{
			//		temp0 = inputSwap(point0, octant);
			//		temp1 = inputSwap(point1, octant);
			//		plotLine(temp0, temp1);
			//	}
			//}

			////Map top
			//for (int i = mapX-1; i > 0; i--)
			//{
			//	point1 = glm::vec2(i, mapY-1);
			//	octant = calculateOctant(point0, point1);
			//	if (octant != 90)
			//	{
			//		temp0 = inputSwap(point0, octant);
			//		temp1 = inputSwap(point1, octant);
			//		plotLine(temp0, temp1);
			//	}
			//}

			////Map left side
			//for (int i = mapY-1; i > 0; i--)
			//{
			//	point1 = glm::vec2(1, i);
			//	octant = calculateOctant(point0, point1);
			//	if (octant != 90)
			//	{
			//		temp0 = inputSwap(point0, octant);
			//		temp1 = inputSwap(point1, octant);
			//		plotLine(temp0, temp1);
			//	}
			//}  
#pragma endregion

		}

		void CameraTestScene::calculateVisionOctant(int playerX, int playerY, int oct)
		{
			point0 = glm::vec2(playerX, playerY);
			
			switch (oct)
			{
			case 0:
			{
				for (int i = mapY / 2; i < mapY; i++)
				{
					point1 = glm::vec2(mapX-1, i);
					octant = calculateOctant(point0, point1);
					if (octant != 90)
					{
						temp0 = inputSwap(point0, octant);
						temp1 = inputSwap(point1, octant);

						plotLine(temp0, temp1);
					}
				}
				break;
			}
			case 1:
			{
				for (int i = mapX-1; i > mapX/2; i--)
				{
					point1 = glm::vec2(i, mapY-1);
					octant = calculateOctant(point0, point1);
					if (octant != 90)
					{
						temp0 = inputSwap(point0, octant);
						temp1 = inputSwap(point1, octant);

						plotLine(temp0, temp1);
					}
				}
				break;
			}
			case 2:
			{
				for (int i = mapX/2; i > 0; i--)
				{
					point1 = glm::vec2(i, mapY-1);
					octant = calculateOctant(point0, point1);
					if (octant != 90)
					{
						temp0 = inputSwap(point0, octant);
						temp1 = inputSwap(point1, octant);

						plotLine(temp0, temp1);
					}
				}
				break;
			}
			case 3:
			{
				for (int i = mapY-1; i > mapY/2; i--)
				{
					point1 = glm::vec2(1, i);
					octant = calculateOctant(point0, point1);
					if (octant != 90)
					{
						temp0 = inputSwap(point0, octant);
						temp1 = inputSwap(point1, octant);

						plotLine(temp0, temp1);
					}
				}
				break;
			}
			case 4:
			{
				for (int i = mapY/2; i > 0; i--)
				{
					point1 = glm::vec2(1, i);
					octant = calculateOctant(point0, point1);
					if (octant != 90)
					{
						temp0 = inputSwap(point0, octant);
						temp1 = inputSwap(point1, octant);

						plotLine(temp0, temp1);
					}
				}
				break;
			}
			case 5:
			{
				for (int i = 1; i < mapX/2; i++)
				{
					point1 = glm::vec2(i, 1);
					octant = calculateOctant(point0, point1);
					if (octant != 90)
					{
						temp0 = inputSwap(point0, octant);
						temp1 = inputSwap(point1, octant);

						plotLine(temp0, temp1);
					}
				}
				break;
			}
			case 6:
			{
				for (int i = mapX / 2; i < mapX-1; i++)
				{
					point1 = glm::vec2(i, 1);
					octant = calculateOctant(point0, point1);
					if (octant != 90)
					{
						temp0 = inputSwap(point0, octant);
						temp1 = inputSwap(point1, octant);

						plotLine(temp0, temp1);
					}
				}
				break;
			}
			case 7:
			{
				for (int i = 1; i < mapY/2; i++)
				{
					point1 = glm::vec2(mapX-1, i);
					octant = calculateOctant(point0, point1);
					if (octant != 90)
					{
						temp0 = inputSwap(point0, octant);
						temp1 = inputSwap(point1, octant);

						plotLine(temp0, temp1);
					}
				}
				break;
			}

			default:
				break;
			}
		}

		void CameraTestScene::calculate90(int playerX, int playerY)
		{
			for (int i = 0; i<10000; i++)
			{
				temp = glm::vec2(playerX, playerY + i);
				if (checkIfVisiblePoint(temp) == true)
				{
					i = 100001; //Wall was hit
				}				
			}
			for (int i = 0; i<10000; i++)
			{
				temp = glm::vec2(playerX, playerY - i);
				if (checkIfVisiblePoint(temp) == true)
				{
					i = 100001; //Wall was hit
				}
			}
		}

		void CameraTestScene::emptyVector(int vectorAsNumber)
		{
			switch (vectorAsNumber)
			{
			case 1:
			{
				for (int i = 0; i < objectTiles.size(); i++)
				{
					objectTiles[i] = 0;
				}
				break;
			}
			case 2:
			{
				for (int i = 0; i < visibleTiles.size(); i++)
				{
					visibleTiles[i] = 0;
				}
				break;
			}
			default:
				break;
			}
		}

		void CameraTestScene::addIntoVector(int vectorAsNumber, glm::vec2 xy, int tiletype)
		{
			switch (vectorAsNumber)
			{
			case 1:
			{
				if (xy.x >= 0 && xy.y >= 0)
				{
					objectTiles[xy.y * mapX + xy.x] = tiletype;
				}
				break;
			}
			case 2:
			{
				if (xy.x >= 0 && xy.y >= 0)
				{
					visibleTiles[xy.y * mapX + xy.x] = tiletype;
				}
				break;
			}
			default:
				break;
			}
		}
	}
}