#include "Pseudo3D.h"

#include <iostream>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Color.h>
#include <glm\gtc\type_ptr.hpp>

namespace Engin
{
	namespace Game
	{
		Pseudo3D::Pseudo3D(Engin* engine) : camera(createWorldCamera()), camera2(createWorldCamera()), camera3(createGuiCamera())
		{
#pragma region INIT
			this->engine = engine;

			std::cout << "Scene started" << std::endl;

			camera->initCamera(0.0f, 0.0f, 800.0f, 800.0f, -1600.0f, 0.0f, 0, 0);
			camera->setZoomLevel(2); //Use zoom if raycasting image smaller than 800. (example: raycast w = 400, zoom = 2, example1: raycast w = 200, zoom = 4) 
			camera2->initCamera(800.0f, 0.0f, 800.0f, 800.0f, 0.0f, 0.0f, 400, 100);
			camera3->initCamera(0.0f, 0.0f, 1600.0f, 800.0f, 0.0f, 0.0f, 800, 400);

			shader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/shader");
			textureShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/texture_shader");
			alphaShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/alpha_shader");
			opaqueBatch.setShader(textureShader);
			alphaBatch.setShader(alphaShader);
			alphaBatch.setSortMode(Renderer::TextureSortMode::FrontToBack);
			guiBatch.setShader(alphaShader);
			guiBatch.setSortMode(Renderer::TextureSortMode::FrontToBack);

			
			furball = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_upside2_64.png");			
			doge1 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_brick_tile_64.png");
			doge2 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_warning_tile_64.png");
			doge3 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_pine_tile_64.png");
			doge4 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_redwood_tile_64.png");
			doge5 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_cubes_tile_64.png");

			furball_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_256.png");
			brick_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_brick_tile_256.png");
			warning_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_warning_tile_256.png");
			pine_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_pine_tile_256.png");
			redwood_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_redwood_tile_256.png");
			cubes_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/wall_cubes_tile_256.png");
			
			animFurball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/furball360_40.xml");
			animPlayer.setAnimation(animFurball360);
			animPlayer.loopable(true);
			animPlayer.pause();

			animFireball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/fireball360_8.xml");
			animPlayer1.setAnimation(animFireball360);
			animPlayer1.loopable(true);
			animPlayer1.setLoopStartFrame(0);
			animPlayer1.setLoopEndFrame(9);
			animPlayer1.start();

			animPlayer12d.setAnimation(animFireball360);
			animPlayer12d.loopable(true);
			animPlayer12d.setLoopStartFrame(20);
			animPlayer12d.setLoopEndFrame(29);
			animPlayer12d.start();

			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(40);
			textCreator.createTextTexture(font, "|", 255, 100, 0);
			text = textCreator.getTexture();
			textCreator3.createTextTexture(font, "|", 255, 100, 0);
			text3 = textCreator3.getTexture();

			mapX = 24;
			mapY = 24;
			tileSize2d = 64;
			alpha = 0.0f;

			//DDA
			tileSize = 256;

			w = 400; //In pseudo3d header change DDAlines size accordingly
			h = 400;

			moveSpeed = 0.11f;
			rotSpeed = 0.02f;
			player = { { 22.0f, 12.0f, 0.0f, 0 , 1} }; //x,y,rotation(radians), howmany sides drawn, spritetype

			dirX = -1, dirY = 0; //initial direction vector
			planeX = 0.0f, planeY = 0.5; //the 2d raycaster version of camera plane

			//adding sprites
			sprite = { { 5.0f, 15.0f, 0.0f, 40 , 1 } }; //x, y, rotation(radians), how many sides, spritetype 
			sprite1 = { { 12.0f, 18.3f, 0.0f, 40, 1 } };
			sprite2 = { { 13.5f, 12.7f, 0.0f, 40, 1 } };
			sprite3 = { {14.0f, 12.0f, 0.0f, 40, 1 } };
			sprite4 = { {15.0f, 13.8f, 0.0f, 40, 1 } };
			sprite5 = { { 16.0f, 4.0f, 0.0f, 40, 1 } };
			sprite6 = { { 18.0f, 13.0f, 0.0f, 40, 1 } };
			sprite7 = { { 12.4f, 14.9f, 0.0f, 40, 1 } };
			sprite8 = { { 23.0f, 15.0f, 0.0f, 40, 1 } };
			sprite9 = { { 12.199f, 16.2f, 0.0f, 40, 1 } };
			fireball = { { 5.0f, 15.0f, 0.0f, 8, 2 } };
			fireball1 = { { 7.0f, 10.0f, 0.0f, 8, 2 } };
			fireball2 = { { 8.0f, 10.0f, 0.0f, 8, 2 } };
			fireball3 = { { 9.0f, 10.0f, 0.0f, 8, 2 } };
			spriteContainer.push_back(sprite);
			spriteContainer.push_back(sprite1);
			spriteContainer.push_back(sprite2);
			spriteContainer.push_back(sprite3);
			spriteContainer.push_back(sprite4);
			spriteContainer.push_back(sprite5);
			spriteContainer.push_back(sprite6);
			spriteContainer.push_back(sprite7);
			spriteContainer.push_back(sprite8);
			spriteContainer.push_back(sprite9);
			spriteContainer.push_back(fireball);
			spriteContainer.push_back(fireball1);
			spriteContainer.push_back(fireball2);
			spriteContainer.push_back(fireball3);

			//filling raycaster lines with 0
			for (int i = 0; i < w; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					DDAlines[i][j] = 0;
				}
			}

			DDASpriteDrawData.resize(spriteContainer.size());

			//filling world with 0
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 25; j++)
				{
					objectTiles[i][j] = 0;
				}				
			}

#pragma endregion

#pragma region WallTiles

			//MapSides
			for (int i = 0; i < mapY; i++)
			{
				addIntoVector(1, glm::vec2(0, i), 1);
			}

			for (int i = 0; i <= mapY; i++)
			{
				addIntoVector(1, glm::vec2(mapX, i), 1);
			}

			for (int i = 0; i < mapX; i++)
			{
				addIntoVector(1, glm::vec2(i, mapY), 1);
			}

			for (int i = 0; i < mapX; i++)
			{
				addIntoVector(1, glm::vec2(i, 0), 3);
			}
			//---------------------

			for (int i = 1; i < 10; i++)
			{
				addIntoVector(1, glm::vec2(i, 10), 2);
			}

			for (int i = 1; i < 13; i++)
			{
				addIntoVector(1, glm::vec2(10, i), 2);
			}

			for (int i = 17; i < 23; i++)
			{
				addIntoVector(1, glm::vec2(i, 17), 4);
			}

			for (int i = 17; i < 23; i++)
			{
				addIntoVector(1, glm::vec2(i, 19), 5);
			}


#pragma endregion
			
		}

		Pseudo3D::~Pseudo3D()
		{
			std::cout << "Scene shutdown" << std::endl;
		}

		void Pseudo3D::update(GLfloat step)
		{
			myTimer.start();
#pragma region DDAMovement
			//move forward if no wall in front of you			
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_W))
			{
				if (objectTiles[int((player[0] + dirX * moveSpeed))][int(player[1])] == false) player[0] += dirX * moveSpeed;
				if (objectTiles[int(player[0])][int(player[1] + dirY * moveSpeed)] == false) player[1] += dirY * moveSpeed;
			}
			//move backwards if no wall behind you
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_S))
			{
				if (objectTiles[int(player[0] - dirX * moveSpeed)][int(player[1])] == false) player[0] -= dirX * moveSpeed;
				if (objectTiles[int(player[0])][int(player[1] - dirY * moveSpeed)] == false) player[1] -= dirY * moveSpeed;
			}
			//strafe left if no wall in left of you			
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_A))
			{
				if (objectTiles[int(player[0] - planeX * moveSpeed)][int(player[1])] == false) player[0] -= planeX * moveSpeed;
				if (objectTiles[int(player[0])][int(player[1] - planeY * moveSpeed)] == false) player[1] -= planeY * moveSpeed;
			}
			//strafe right if no wall in right of you			
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_D))
			{
				if (objectTiles[int((player[0] + planeX * moveSpeed))][int(player[1])] == false) player[0] += planeX * moveSpeed;
				if (objectTiles[int(player[0])][int(player[1] + planeY * moveSpeed)] == false) player[1] += planeY * moveSpeed;
			}
			//rotate to the right   
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_RIGHT))
			{
				//both camera direction and camera plane must be rotated
				double oldDirX = dirX;
				dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
				dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
				double oldPlaneX = planeX;
				planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
				planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
			}
			//rotate to the left
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_LEFT))
			{
				//both camera direction and camera plane must be rotated
				double oldDirX = dirX;
				dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
				dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
				double oldPlaneX = planeX;
				planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
				planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
			}
#pragma endregion
			alpha += 0.01;			
			
			//Raycast calculations.
			DDA();
			DDADrawSprites();

			//Saving player rotation
			player[2] = -glm::atan(dirX, dirY); //check camera.cpp rotation direction
			
			//rotating sprites in radians
			spriteContainer[5][2] += 0.05f;
			spriteContainer[0][2] += 0.01f;
			spriteContainer[4][2] += 0.03f;

			//moving sprites TODO: Make some logic and translate sprites with them.
			spriteContainer[0][0] = 5.0f + cos(spriteContainer[0][2]);
			spriteContainer[0][1] = 15.0f + sin(spriteContainer[0][2]);

			spriteContainer[4][0] = 15.0f + 3 * glm::cos(alpha);
			spriteContainer[6][1] = 10.0f + 4 * glm::sin(alpha);

			//moving fireballs
			static float firex = 10.0f;
			if (firex < 24.0f)
			{
				firex += 0.05f;				
			}
			else
			{
				firex = 10.0f;
			}
			spriteContainer[11][1] = firex;
			spriteContainer[12][1] = firex;
			spriteContainer[13][1] = firex;

			//2d camera
			camera2->setPositionRotationOrigin((player[0]*tileSize2d) + 800, (player[1]*tileSize2d));
			camera2->setRotation(glm::degrees(player[2]));

			//Information		
			textCreator3.createTextTexture(font, "WASD + arrows " + std::to_string(player[0]) + " " + std::to_string(player[1]), 255, 100, 0);
			text3 = textCreator3.getTexture();
			myTimer.pause();
			textCreator.createTextTexture(font, "Update calculation time: " + std::to_string(myTimer.getLocalTime()) + " ms", 255, 100, 0);
			text = textCreator.getTexture();
					
			animPlayer12d.update();
		}

		void Pseudo3D::interpolate(GLfloat alpha)
		{
		}

		void Pseudo3D::draw()
		{
			//DDA draw test
#pragma region WallTileDraw
			for (int i = 0; i < DDAlines.size(); i++)
			{
				if (DDAlines[i][2] > 0)
				{
					if (DDAlines[i][1] <= 0)
					{
						depth = 0.99f;
					}
					else
					{
						depth = (1.0f / DDAlines[i][1]);
					}


					switch (int(DDAlines[i][3]))
					{
					case 1:
					{
						opaqueBatch.draw(brick_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, depth);
						break;
					}

					case 2:
					{
						opaqueBatch.draw(warning_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, depth);
						break;
					}
					case 3:
					{
						opaqueBatch.draw(pine_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, depth);

						break;
					}
					case 4:
					{
						opaqueBatch.draw(redwood_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, depth);

						break;
					}
					case 5:
					{
						opaqueBatch.draw(cubes_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, depth);

						break;
					}

					//shade colors
					case 6:
					{
						opaqueBatch.draw(brick_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, { 0.5f, 0.5f, 0.5f }, 1.0f, depth);
						break;
					}

					case 7:
					{
						opaqueBatch.draw(warning_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, { 0.5f, 0.5f, 0.5f }, 1.0f, depth);
						break;
					}
					case 8:
					{
						opaqueBatch.draw(pine_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, { 0.5f, 0.5f, 0.5f }, 1.0f, depth);

						break;
					}
					case 9:
					{
						opaqueBatch.draw(redwood_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, { 0.5f, 0.5f, 0.5f }, 1.0f, depth);
						break;
					}
					case 10:
					{
						opaqueBatch.draw(cubes_256, &glm::vec4(DDAlines[i][4], 0.0f, 1.0f, tileSize), DDAlines[i][0] - 1600, DDAlines[i][1], 1.0f, DDAlines[i][2] - DDAlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, { 0.5f, 0.5f, 0.5f }, 1.0f, depth);
						break;
					}

					default:
						break;
					}
				}
			}
#pragma endregion
			
			//"3D" sprites
			if (DDASpriteDrawData.size() > 0)
			{
				for (int i = 0; i < DDASpriteDrawData.size(); i++)
				{
					if (DDASpriteDrawData[i][0] > -w && DDASpriteDrawData[i][0] < (w + tileSize) && DDASpriteDrawData[i][3]>0)
					{
						if (DDASpriteDrawData[i][1] <= 0)
						{
							depth = h; //disappear
						}
						else
						{
							depth = (1.0f / DDASpriteDrawData[i][1]);
						}						
						
						if (int(spriteContainer[i][4]) == 1) //furball with different directions
						{					
							animPlayer.setCurrentFrame(int(DDASpriteDrawData[i][4]));
							alphaBatch.draw(animPlayer.getTexture(), animPlayer.getCurrentFrameTexCoords(),
								DDASpriteDrawData[i][0] - 1600, DDASpriteDrawData[i][1],
								256, 256, 0.0f, 0.0f, 0.0f,
								DDASpriteDrawData[i][2], Renderer::clrWhite, 1.0f, depth);
						}
						if (int(spriteContainer[i][4]) == 2) //animated flame
						{							
							spriteStartFrame = int(DDASpriteDrawData[i][4]) * 10;
							spriteEndFrame = int(DDASpriteDrawData[i][4]) * 10 + 9;		
							
							animPlayer1.setLoopStartFrame(spriteStartFrame);						
							animPlayer1.setLoopEndFrame(spriteEndFrame);
							animPlayer1.update(); //Alternative: create own animplayer for every fireball
							
							alphaBatch.draw(animPlayer1.getTexture(), animPlayer1.getCurrentFrameTexCoords(),
								DDASpriteDrawData[i][0] - 1600, DDASpriteDrawData[i][1], 256, 256, 0.0f, 0.0f, 0.0f,
								DDASpriteDrawData[i][2], Renderer::clrWhite, 1.0f, depth);
						}						
					}
				}
			}			
			//---------------

			//2D camera draw
			for (int i = 0; i <= mapY; i++)
			{
				for (int j = 0; j <= mapX; j++)
				{
					float offset = 32.0f;

					if (objectTiles[j][i] == 1)
					{
						opaqueBatch.draw(doge1, (j * tileSize2d) + 800 + offset, i * tileSize2d + offset, 1.0f, 0.4f);
					}
					else if (objectTiles[j][i] == 2)
					{
						opaqueBatch.draw(doge2, (j * tileSize2d) + 800 + offset, i * tileSize2d + offset, 1.0f, 0.4f);
					}
					else if (objectTiles[j][i] == 3)
					{
						opaqueBatch.draw(doge3, (j * tileSize2d) + 800 + offset, i * tileSize2d + offset, 1.0f, 0.4f);
					}
					else if (objectTiles[j][i] == 4)
					{
						opaqueBatch.draw(doge4, (j * tileSize2d) + 800 + offset, i * tileSize2d + offset, 1.0f, 0.4f);
					}
					else if (objectTiles[j][i] == 5)
					{
						opaqueBatch.draw(doge5, (j * tileSize2d) + 800 + offset, i * tileSize2d + offset, 1.0f, 0.4f);
					}
				}				
			}
			for (int i = 0; i < spriteContainer.size(); i++)
			{
				if (int(spriteContainer[i][4]) == 1)
				{
					alphaBatch.draw(furball, &glm::vec4(0.0f, 0.0f, furball->getWidth(), furball->getHeight()),
						spriteContainer[i][0] * tileSize2d + 800, spriteContainer[i][1] * tileSize2d, furball->getWidth(), 
						furball->getHeight(), tileSize2d / 2, tileSize2d / 2, glm::degrees(spriteContainer[i][2]), 1.0f, Renderer::clrWhite, 1.0f, 0.7f + i*0.01f);
				}

				else if (int(spriteContainer[i][4]) == 2)
				{
					alphaBatch.draw(animPlayer12d.getTexture(), animPlayer12d.getCurrentFrameTexCoords(),
						spriteContainer[i][0] * tileSize2d + 800, spriteContainer[i][1] * tileSize2d, 256, 256, 256 / 2, 256 / 2, glm::degrees(spriteContainer[i][2]) + 90.0f,
						0.25f, Renderer::clrWhite, 1.0f, 0.8f + i * 0.01f);
				}
			}

			//player
			alphaBatch.draw(furball, &glm::vec4(0.0f, 0.0f, furball->getWidth(), furball->getHeight()), player[0]*tileSize2d + 800, player[1]*tileSize2d, furball->getWidth(), furball->getHeight(), tileSize2d / 2, tileSize2d / 2, glm::degrees(player[2]), 1.0f, Renderer::clrRed, 1.0f, 0.8f);

			//---------------

			//Hud
			guiBatch.draw(text, &glm::vec4(0.0f, 0.0f, text->getWidth(), text->getHeight()), camera3->getPositionRotationOrigin()[0] + glm::cos(alpha)*400.0f, camera3->getPositionRotationOrigin()[1] + 350, text->getWidth(), text->getHeight(), text->getWidth() / 2, text->getHeight() / 2, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 1.0f);
			guiBatch.draw(text3, &glm::vec4(0.0f, 0.0f, text3->getWidth(), text3->getHeight()), camera3->getPositionRotationOrigin()[0], camera3->getPositionRotationOrigin()[1] + 300, text3->getWidth(), text3->getHeight(), text3->getWidth() / 2, text3->getHeight() / 2, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 1.0f);
		}

		void Pseudo3D::addIntoVector(int vectorAsNumber, glm::vec2 xy, int tiletype)
		{
			switch (vectorAsNumber)
			{
			case 1:
			{
				if (xy.x >= 0 && xy.y >= 0)
				{
					objectTiles[xy.x][xy.y] = tiletype;
				}
				break;
			}
			default:
				break;
			}
		}

		void Pseudo3D::DDA() //Vector operations are slow, changed to std::arrays, change array sizes accordingly in the Pseudo3D header.
		{
			for (int x = 0; x < w; x++)
			{
				//calculate ray position and direction 
				cameraX = 2 * x / double(w) - 1; //x-coordinate in camera space
				rayPosX = player[0];
				rayPosY = player[1];
				rayDirX = dirX + planeX * cameraX;
				rayDirY = dirY + planeY * cameraX;

				//which box of the map we're in  
				DDAX = int(rayPosX);
				DDAY = int(rayPosY);

				//length of ray from one x or y-side to next x or y-side
				deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
				deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
				perpWallDist;

				//what direction to step in x or y-direction (either +1 or -1)
				hit = 0; //was there a wall hit?

				//calculate step and initial sideDist
				if (rayDirX < 0)
				{
					stepX = -1;
					sideDistX = (rayPosX - DDAX) * deltaDistX;
				}
				else
				{
					stepX = 1;
					sideDistX = (DDAX + 1.0 - rayPosX) * deltaDistX;
				}
				if (rayDirY < 0)
				{
					stepY = -1;
					sideDistY = (rayPosY - DDAY) * deltaDistY;
				}
				else
				{
					stepY = 1;
					sideDistY = (DDAY + 1.0 - rayPosY) * deltaDistY;
				}

				//perform DDA
				while (hit == 0)
				{
					//jump to next map square, OR in x-direction, OR in y-direction
					if (sideDistX < sideDistY)
					{
						sideDistX += deltaDistX;
						DDAX += stepX;
						side = 0;
					}
					else
					{
						sideDistY += deltaDistY;
						DDAY += stepY;
						side = 1;
					}
					//Check if ray has hit a wall
					if (objectTiles[DDAX][DDAY] > 0)
					{
						hit = 1;
					}
					//if (worldMap[mapX][mapY] > 0) hit = 1;
				}

				//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
				if (side == 0)
					perpWallDist = fabs((DDAX - rayPosX + (1 - stepX) / 2) / rayDirX);
				else
					perpWallDist = fabs((DDAY - rayPosY + (1 - stepY) / 2) / rayDirY);

				//Calculate height of line to draw on screen
				lineHeight = glm::abs(int(h / perpWallDist));

				//calculate lowest and highest pixel to fill in current stripe
				drawStart = -lineHeight / 2 + h / 2;
				if (drawStart < 0)drawStart = 0;
				drawEnd = lineHeight / 2 + h / 2;
				if (drawEnd >= h)drawEnd = h - 1;

				//texturing calculations
				texNum = objectTiles[DDAX][DDAY] - 1; //1 subtracted from it so that texture 0 can be used!

				//calculate value of wallX
				wallX; //where exactly the wall was hit
				if (side == 1) wallX = rayPosX + ((DDAY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
				else       wallX = rayPosY + ((DDAX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
				wallX -= floor((wallX));

				//x coordinate on the texture
				texX = int(wallX * double(tileSize));
				if (side == 0 && rayDirX > 0) texX = tileSize - texX - 1;
				if (side == 1 && rayDirY < 0) texX = tileSize - texX - 1;

				//choose wall color			
				switch (objectTiles[DDAX][DDAY])
				{
				case 1:  drawColor = 1;  break; //red
				case 2:  drawColor = 2;  break; //green
				case 3:  drawColor = 3;   break; //blue
				case 4:  drawColor = 4;  break; //white
				default: drawColor = 5; break; //yellow
				}
				if (side == 1) { drawColor += 5; }
				
				//Saving calculated data for camera slice 
				DDAlines[x][0] = x;
				DDAlines[x][1] = drawStart;
				DDAlines[x][2] = drawEnd;
				DDAlines[x][3] = drawColor;
				DDAlines[x][4] = texX;
				//---------------------------------------
			}			
		}

		void Pseudo3D::DDADrawSprites()
		{
			for (int i = 0; i < spriteContainer.size(); i++)
			{
				spriteX = spriteContainer[i][0] - player[0];
				spriteY = spriteContainer[i][1] - player[1];

				glm::vec2 transform;

				//Creating sprite transform vector
				transform = glm::vec2(spriteX, spriteY) * glm::inverse(glm::mat2x2(planeX,dirX,planeY,dirY));				

				//Calculating camera y and scale for the sprite
				spriteHeightWidth = glm::abs(h / transform.y);
				spriteYout = -spriteHeightWidth / 2 + h / 2;
				spriteScale = 1 / (tileSize / spriteHeightWidth);

				//Calculating x position for the sprite				
				spriteScreenX = (w / 2)*(1 + transform.x / transform.y);
				spriteXout = -spriteHeightWidth / 2 + spriteScreenX;

				//Calculating sprite side, -90 degrees correction has to made.				
				spriteAngle = (glm::atan(-spriteX , -spriteY));
				if (spriteAngle < 0.0f)
				{
					spriteAngle += glm::radians(360.0f);
				}

				//Sprites own facing changes the angle.
				if (spriteContainer[i][2] != 0.0f)
				{
					convertToFloat = float(spriteContainer[i][2]);
					spriteFacing = glm::mod((-convertToFloat + glm::radians(360.0f)), glm::radians(360.0f));
					spriteAngle -= spriteFacing;

					if (spriteAngle < 0.0f)
					{
						spriteAngle += glm::radians(360.0f);
					}
				}
				
				spriteAnimIndex = getSpriteAnimIndex(spriteAngle, spriteContainer[i][3]);

				//Saving data
				DDASpriteDrawData[i][0] = spriteXout;
				DDASpriteDrawData[i][1] = spriteYout;
				DDASpriteDrawData[i][2] = spriteScale;
				DDASpriteDrawData[i][3] = transform.y;
				DDASpriteDrawData[i][4] = spriteAnimIndex;
			}			
		}

		int Pseudo3D::getSpriteAnimIndex(double angle, double sides)
		{
			spriteSideAngle = 360.0f / sides;

			if (glm::degrees(angle) < (spriteSideAngle/2.0f))
			{
				return 0;
			}
			else if (glm::degrees(angle) >= (360.0f - (spriteSideAngle / 2.0f)))
			{
				return 0;
			}
			else
			{
				return (glm::degrees(angle) + (spriteSideAngle / 2.0f)) / spriteSideAngle;
			}
		}
	}
}