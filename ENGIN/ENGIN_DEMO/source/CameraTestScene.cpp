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
			doge4 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_tile_40.png");
			doge5 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/green_tile_40.png");

			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(40);
			textCreator.createTextTexture(font, "Vision calculation time: ", 255,100,0);
			text = textCreator.getTexture();
			/*textCreator2.createTextTexture(font, "Scene draw time: ", 255, 100, 0);
			text2 = textCreator2.getTexture();*/
			textCreator3.createTextTexture(font, "Visible tile count: ", 255, 100, 0);
			text3 = textCreator3.getTexture();
			
#pragma endregion

#pragma region WallTiles
			wallTiles.push_back(glm::vec2(1, 18));
			wallTiles.push_back(glm::vec2(2, 19));
			wallTiles.push_back(glm::vec2(2, 18));
			wallTiles.push_back(glm::vec2(1, 17));
			wallTiles.push_back(glm::vec2(3, 19));
			
			wallTiles.push_back(glm::vec2(1, 1));

			wallTiles.push_back(glm::vec2(8, 5));
			wallTiles.push_back(glm::vec2(8, 4));


			for (int i = 10; i < 15; i++)
			{
				wallTiles.push_back(glm::vec2(i, 18));
			}

			for (int i = 13; i < 18; i++)
			{
				wallTiles.push_back(glm::vec2(2, i));
			}

			for (int i = 11; i < 16; i++)
			{
				wallTiles.push_back(glm::vec2(0, i));
			}

			for (int i = 1; i < 11; i++)
			{
				wallTiles.push_back(glm::vec2(i, 11));
			}

			for (int i = 2; i < 11; i++)
			{
				wallTiles.push_back(glm::vec2(i, 9));
			}


			for (int i = -1; i < 22; i++)
			{
				wallTiles.push_back(glm::vec2(-1, i));
			}

			for (int i = -1; i < 22; i++)
			{
				wallTiles.push_back(glm::vec2(21, i));
			}

			for (int i = -1; i < 22; i++)
			{
				wallTiles.push_back(glm::vec2(i, 21));
			}

			for (int i = -1; i < 22; i++)
			{
				wallTiles.push_back(glm::vec2(i, -1));
			}

			for (int i = 15; i < 19; i++)
			{
				wallTiles.push_back(glm::vec2(i, 3));
			}

			for (int i = 4; i < 7; i++)
			{
				wallTiles.push_back(glm::vec2(18, i));
			}

			for (int i = 8; i < 17; i++)
			{
				wallTiles.push_back(glm::vec2(15, i));
			}

			for (int i = 12; i < 16; i++)
			{
				wallTiles.push_back(glm::vec2(i, 7));
			}

			for (int i = 2; i < 13; i++)
			{
				wallTiles.push_back(glm::vec2(i, 3));
			}
#pragma endregion

			//At the beginning:
			calculateVision(400, 400);
			calculate90(400,400);
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

			static int playerX = 400;
			static int playerY = 400;
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_W))
			{
				//std::cout << "W" << std::endl;
				if (playerY < 800)
				{
					playerY += 40;
					//visibleTiles.clear();
					//calculateVision(playerX, playerY);
					//calculate90(playerX, playerY);
				}
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_A))
			{
				//std::cout << "A" << std::endl;
				if (playerX > 0)
				{
					playerX -= 40;
					//visibleTiles.clear();
					//calculateVision(playerX, playerY);
					//calculate90(playerX, playerY);
				}
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_S))
			{
				//std::cout << "S" << std::endl;
				if (playerY > 0)
				{
					playerY -= 40;
					//visibleTiles.clear();
					//calculateVision(playerX, playerY);
					//calculate90(playerX, playerY);
				}
			}
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_D))
			{
				//std::cout << "D" << std::endl;
				if (playerX < 800)
				{
					playerX += 40;
					//visibleTiles.clear();
					//calculateVision(playerX, playerY);
					//calculate90(playerX, playerY);
				}
			}

			camera.setPositionRotationOrigin(playerX, playerY);

			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_SPACE))
			{
				std::cout << camera.getPositionRotationOrigin().x << " - " << camera.getPositionRotationOrigin().y <<" visible tiles: "<< visibleTiles.size()<< std::endl;
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
			visibleTiles.clear();
			visibleFriend.clear();
			calculateVision(playerX, playerY);
			calculate90(playerX, playerY);
			

			friendX = 17;
			friendY = 10;

			for (int i = 0; i < visibleTiles.size(); i++)
			{
				if (visibleTiles[i].x == friendX && visibleTiles[i].y == friendY)
				{
					visibleFriend.push_back(glm::vec2(friendX, friendY));
				}
			}

			myTimer.pause();

			//Information
			textCreator.createTextTexture(font, "Vision calculation time: " + std::to_string(myTimer.getLocalTime()) + " ms", 255, 100, 0);
			text = textCreator.getTexture();
			/*textCreator2.createTextTexture(font, "Scene draw time: " + std::to_string(myTimer2.getLocalTime()) + " ms", 255, 100, 0);
			text2 = textCreator2.getTexture();*/
			textCreator3.createTextTexture(font, "Visible tiles count: " + std::to_string(visibleTiles.size()), 255, 100, 0);
			text3 = textCreator3.getTexture();
		}

		void CameraTestScene::interpolate(GLfloat alpha)
		{
		}

		void CameraTestScene::draw()
		{
			//myTimer2.start();
			textureBatch.begin();
			alphaTextureBatch.begin();
			
			alphaTextureBatch.draw(doge2, camera.getPositionRotationOrigin().x, camera.getPositionRotationOrigin().y, 1.0f, 0.6f);
			
			//friend
			alphaTextureBatch.draw(doge2, friendX*40, friendY*40, 1.0f, 0.6f);

			
			if (visibleTiles.size() > 0)
			{
				for (int i = 0; i < visibleTiles.size(); i++)
				{
					alphaTextureBatch.draw(doge3, visibleTiles[i].x * 40, visibleTiles[i].y * 40, 0.50f, 0.5f);
				}
			}

			if (visibleFriend.size() > 0)
			{
				for (int i = 0; i < visibleFriend.size(); i++)
				{
					alphaTextureBatch.draw(doge5, visibleFriend[i].x * 40, visibleFriend[i].y * 40, 0.50f, 0.9f);
				}
			}

			if (wallTiles.size() > 0)
			{
				for (int i = 0; i < wallTiles.size(); i++)
				{
					alphaTextureBatch.draw(doge4, wallTiles[i].x * 40, wallTiles[i].y * 40, 1.0f, 0.5f);
				}
			}
			

			alphaTextureBatch.draw(text, camera.getPositionRotationOrigin().x - 60.0f, camera.getPositionRotationOrigin().y + 360.0f, 1.0f, 0.9f);
			//alphaTextureBatch.draw(text2, camera.getPositionRotationOrigin().x - 100.0f, camera.getPositionRotationOrigin().y + 310.0f, 1.0f, 0.9f);
			alphaTextureBatch.draw(text3, camera.getPositionRotationOrigin().x - 100.0f, camera.getPositionRotationOrigin().y + 270.0f, 1.0f, 0.9f);
			
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

			//myTimer2.pause();
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

			if (addVisiblePoint(this->point0) == true)
			{
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
				if (addVisiblePoint(glm::vec2(temp)) == true)
				{
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

#pragma region Withangles
			//alpha = glm::atan(point1.y - point0.y, point1.x - point0.x);

			//if (alpha < 0)
			//{
			//	alpha = glm::radians(360.0f) + alpha;
			//}

			//if (alpha >= glm::radians(0.0f) && alpha < glm::radians(44.9999f))
			//{
			//	return 0;
			//}
			//if (alpha > glm::radians(45.0001f) && alpha <= glm::radians(90.0f))
			//{
			//	return 1;
			//}
			//if (alpha >= glm::radians(90.0f) && alpha < glm::radians(134.9999f))
			//{
			//	return 2;
			//}
			//if (alpha > glm::radians(135.0001f) && alpha <= glm::radians(180.0f))
			//{
			//	return 3;
			//}
			//if (alpha >= glm::radians(180.0f) && alpha < glm::radians(224.9999f))
			//{
			//	return 4;
			//}
			//if (alpha > glm::radians(225.0001f) && alpha <= glm::radians(270.0f))
			//{
			//	return 5;
			//}
			//if (alpha >= glm::radians(270.0f) && alpha < glm::radians(314.9999f))
			//{
			//	return 6;
			//}
			//if (alpha > glm::radians(315.0001f) && alpha <= glm::radians(360.0f))
			//{
			//	return 7;
			//}
			//else //45, 135, 225, 315
			//{
			//	return 45;
			//}		  
#pragma endregion

		}

		bool CameraTestScene::addVisiblePoint(glm::vec2 point)
		{
			int count = 0;
			int count1 = 0;

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

			if (wallTiles.size() > 0)
			{
				for (int i = 0; i < wallTiles.size(); i++)
				{
					if (wallTiles[i] == point)
					{
						count1++;
					}
				}
			}

			if (count == 0)
			{
				if (count1 == 0)
				{
					visibleTiles.push_back(point);
				}
				else
				{
					return true; //Wall was hit
				}				
			}
			return false;
		}
		
		void CameraTestScene::calculateVision(int playerX,int playerY)
		{
			point0 = glm::vec2(playerX / 40, playerY / 40);
			
			////Map bottom
			for (int i = 0; i < 20; i++)
			{
				point1 = glm::vec2(i, 0);
				octant = calculateOctant(point0, point1);
				if (octant != 90)
				{
					temp0 = inputSwap(point0, octant);
					temp1 = inputSwap(point1, octant);

					plotLine(temp0, temp1);
				}				
			}

			//Map right side
			for (int i = 0; i <= 20; i++)
			{
				point1 = glm::vec2(20, i);
				octant = calculateOctant(point0, point1);
				if (octant != 90)
				{
					temp0 = inputSwap(point0, octant);
					temp1 = inputSwap(point1, octant);
					plotLine(temp0, temp1);
				}
			}

			//Map top
			for (int i = 20; i >= 0; i--)
			{
				point1 = glm::vec2(i, 20);
				octant = calculateOctant(point0, point1);
				if (octant != 90)
				{
					temp0 = inputSwap(point0, octant);
					temp1 = inputSwap(point1, octant);
					plotLine(temp0, temp1);
				}
			}

			//Map left side
			for (int i = 20; i >= 0; i--)
			{
				point1 = glm::vec2(0, i);
				octant = calculateOctant(point0, point1);
				if (octant != 90)
				{
					temp0 = inputSwap(point0, octant);
					temp1 = inputSwap(point1, octant);
					plotLine(temp0, temp1);
				}
			}
		}

		void CameraTestScene::calculate90(int playerX, int playerY)
		{
			for (int i = 0; i<100; i++)
			{
				temp = glm::vec2(playerX / 40, playerY / 40 + i);
				if (addVisiblePoint(temp) == true)
				{
					i = 1000; //Wall was hit
				}				
			}
			for (int i = 0; i<100; i++)
			{
				temp = glm::vec2(playerX / 40, playerY / 40 - i);
				if (addVisiblePoint(temp) == true)
				{
					i = 1000; //Wall was hit
				}
			}
#pragma region CalcX
//			endX = 0;
//			for (endX; endX < 10000; endX++) //Until hit the wall
//			{
//				temp = glm::vec2(playerX / 40 + endX, playerY / 40 + endX);
//				if (addVisiblePoint(temp) == true)
//				{
//					endX = 10001;
//				}
//			}
//			endX = 0;
//			for (endX; endX < 10000; endX++)
//			{
//				temp = glm::vec2(playerX / 40 + endX, playerY / 40 - endX);
//				if (addVisiblePoint(temp) == true)
//				{
//					endX = 10001;
//				}
//			}
//			endX = 0;
//			for (endX; endX < 10000; endX++)
//			{
//				temp = glm::vec2(playerX / 40 - endX, playerY / 40 + endX);
//				if (addVisiblePoint(temp) == true)
//				{
//					endX = 10001;
//				}
//			}
//			for (int i = 0;; i++)
//			{
//				temp = glm::vec2(playerX / 40 - i, playerY / 40 - i);
//				if (addVisiblePoint(temp) == true)
//				{
//					return;
//				}
//			}
#pragma endregion
		}
	}
}