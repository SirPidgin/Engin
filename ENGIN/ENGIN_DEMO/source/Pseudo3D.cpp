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

			
			furball = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_upside_64.png");			
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
			
			animFurball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/furball360.xml");
			animPlayer.setAnimation(animFurball360);
			animPlayer.loopable(true);
			animPlayer.pause();

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
			player = glm::vec3(22.0f, 12.0f, 0.0f);

			dirX = -1, dirY = 0; //initial direction vector
			planeX = 0.0f, planeY = 0.5; //the 2d raycaster version of camera plane

			//adding sprites
			sprite = glm::vec3(12.0f, 12.0f, 0.0f);
			sprite1 = glm::vec3(12.0f, 18.3f, 0.0f);
			sprite2 = glm::vec3(13.5f, 12.7f, 0.0f);
			sprite3 = glm::vec3(14.0f, 12.0f, 0.0f);
			sprite4 = glm::vec3(15.0f, 13.2f, 0.0f);
			sprite5 = glm::vec3(16.0f, 12.0f, 0.0f);
			sprite6 = glm::vec3(18.0f, 13.0f, 0.0f);
			sprite7 = glm::vec3(12.4f, 14.9f, 0.0f);
			sprite8 = glm::vec3(23.0f, 15.0f, 0.0f);
			sprite9 = glm::vec3(12.199f, 16.2f, 0.0f);
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

			for (int i = 1; i < 10; i++)
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
				if (objectTiles[int((player.x + dirX * moveSpeed))][int(player.y)] == false) player.x += dirX * moveSpeed;
				if (objectTiles[int(player.x)][int(player.y + dirY * moveSpeed)] == false) player.y += dirY * moveSpeed;
			}
			//move backwards if no wall behind you
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_S))
			{
				if (objectTiles[int(player.x - dirX * moveSpeed)][int(player.y)] == false) player.x -= dirX * moveSpeed;
				if (objectTiles[int(player.x)][int(player.y - dirY * moveSpeed)] == false) player.y -= dirY * moveSpeed;
			}
			//strafe left if no wall in left of you			
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_A))
			{
				if (objectTiles[int(player.x - planeX * moveSpeed)][int(player.y)] == false) player.x -= planeX * moveSpeed;
				if (objectTiles[int(player.x)][int(player.y - planeY * moveSpeed)] == false) player.y -= planeY * moveSpeed;
			}
			//strafe right if no wall in right of you			
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_D))
			{
				if (objectTiles[int((player.x + planeX * moveSpeed))][int(player.y)] == false) player.x += planeX * moveSpeed;
				if (objectTiles[int(player.x)][int(player.y + planeY * moveSpeed)] == false) player.y += planeY * moveSpeed;
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
			player.z = -glm::atan(dirX, dirY); //check camera.cpp rotation direction
			
			//rotating sprite
			spriteContainer[5].z += 0.01;

			//2d camera
			camera2->setPositionRotationOrigin((player.x*tileSize2d) + 800, (player.y*tileSize2d));
			camera2->setRotation(glm::degrees(player.z));

			//Information		
			textCreator3.createTextTexture(font, "WASD + arrows " + std::to_string(player.x) + " " + std::to_string(player.y), 255, 100, 0);
			text3 = textCreator3.getTexture();
			myTimer.pause();
			textCreator.createTextTexture(font, "Update calculation time: " + std::to_string(myTimer.getLocalTime()) + " ms", 255, 100, 0);
			text = textCreator.getTexture();
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
			
			if (DDASpriteDrawData.size() > 0)
			{
				for (int i = 0; i < DDASpriteDrawData.size(); i++)
				{
					if (DDASpriteDrawData[i][0] > -w && DDASpriteDrawData[i][0] < (w + tileSize) && DDASpriteDrawData[i][3]>0)
					{
						if (DDASpriteDrawData[i][1] <= 0)
						{
							depth = h;
						}
						else
						{
							depth = (1.0f / DDASpriteDrawData[i][1]);
						}		
						animPlayer.setCurrentFrame(int(DDASpriteDrawData[i][4]));
						alphaBatch.draw(animPlayer.getTexture(), animPlayer.getCurrentFrameTexCoords(),
							DDASpriteDrawData[i][0] - 1600, DDASpriteDrawData[i][1],
							256, 256, 0.0f, 0.0f, 0.0f,
							DDASpriteDrawData[i][2], Renderer::clrWhite, 1.0f, depth);
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
				alphaBatch.draw(furball, &glm::vec4(0.0f, 0.0f, furball->getWidth(), furball->getHeight()), spriteContainer[i].x*tileSize2d + 800, spriteContainer[i].y*tileSize2d, furball->getWidth(), furball->getHeight(), tileSize2d / 2, tileSize2d / 2, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 0.7f+i*0.01f);
			}

			//player
			alphaBatch.draw(furball, &glm::vec4(0.0f, 0.0f, furball->getWidth(), furball->getHeight()), player.x*tileSize2d + 800, player.y*tileSize2d, furball->getWidth(), furball->getHeight(), tileSize2d / 2, tileSize2d / 2, camera2->getRotation()+90, 1.0f, Renderer::clrRed, 1.0f, 0.9f);

			//---------------

			//Hud
			guiBatch.draw(text, &glm::vec4(0.0f, 0.0f, text->getWidth(), text->getHeight()), camera3->getPositionRotationOrigin().x + glm::cos(alpha)*400.0f, camera3->getPositionRotationOrigin().y + 350, text->getWidth(), text->getHeight(), text->getWidth() / 2, text->getHeight() / 2, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 1.0f);
			guiBatch.draw(text3, &glm::vec4(0.0f, 0.0f, text3->getWidth(), text3->getHeight()), camera3->getPositionRotationOrigin().x, camera3->getPositionRotationOrigin().y + 300, text3->getWidth(), text3->getHeight(), text3->getWidth() / 2, text3->getHeight() / 2, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 1.0f);
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
				rayPosX = player.x;
				rayPosY = player.y;
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
				spriteX = spriteContainer[i].x - player.x;
				spriteY = spriteContainer[i].y - player.y;

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
				spriteAngle = (glm::atan(-spriteX , -spriteY) - glm::radians(90.0f));
				if (spriteAngle < 0.0f)
				{
					spriteAngle += glm::radians(360.0f);
				}

				//Sprites own facing changes the angle. TODO: fix
				spriteFacing = glm::mod((spriteContainer[i].z + glm::radians(360.0f)), glm::radians(360.0f));
				spriteAngle -= spriteFacing;
				
#pragma region AngleSideCheck
				if (spriteAngle >= glm::radians(348.75f) && spriteAngle <= glm::radians(360.0f) || spriteAngle >= 0.0f && spriteAngle <= glm::radians(11.25f))
				{
					spriteAnimIndex = 0;
				}
				else if (spriteAngle > glm::radians(11.25f) && spriteAngle <= glm::radians(33.75f))
				{
					spriteAnimIndex = 1;
				}
				else if (spriteAngle > glm::radians(33.75f) && spriteAngle <= glm::radians(56.25f))
				{
					spriteAnimIndex = 2;
				}
				else if (spriteAngle > glm::radians(56.25f) && spriteAngle <= glm::radians(78.75f))
				{
					spriteAnimIndex = 3;
				}
				else if (spriteAngle > glm::radians(78.75f) && spriteAngle <= glm::radians(101.25f))
				{
					spriteAnimIndex = 4;
				}
				else if (spriteAngle > glm::radians(101.25f) && spriteAngle <= glm::radians(123.75f))
				{
					spriteAnimIndex = 5;
				}
				else if (spriteAngle > glm::radians(123.75f) && spriteAngle <= glm::radians(146.25f))
				{
					spriteAnimIndex = 6;
				}
				else if (spriteAngle > glm::radians(146.25f) && spriteAngle <= glm::radians(168.75f))
				{
					spriteAnimIndex = 7;
				}
				else if (spriteAngle > glm::radians(168.75f) && spriteAngle <= glm::radians(191.25f))
				{
					spriteAnimIndex = 8;
				}
				else if (spriteAngle > glm::radians(191.25f) && spriteAngle <= glm::radians(213.75f))
				{
					spriteAnimIndex = 9;
				}
				else if (spriteAngle > glm::radians(213.75f) && spriteAngle <= glm::radians(236.25f))
				{
					spriteAnimIndex = 10;
				}
				else if (spriteAngle > glm::radians(236.25f) && spriteAngle <= glm::radians(258.75f))
				{
					spriteAnimIndex = 11;
				}
				else if (spriteAngle > glm::radians(258.75f) && spriteAngle <= glm::radians(281.25f))
				{
					spriteAnimIndex = 12;
				}
				else if (spriteAngle > glm::radians(281.25f) && spriteAngle <= glm::radians(303.75f))
				{
					spriteAnimIndex = 13;
				}
				else if (spriteAngle > glm::radians(303.75f) && spriteAngle <= glm::radians(326.25f))
				{
					spriteAnimIndex = 14;
				}
				else
				{
					spriteAnimIndex = 15;
				}
#pragma endregion


				//Saving data
				DDASpriteDrawData[i][0] = spriteXout;
				DDASpriteDrawData[i][1] = spriteYout;
				DDASpriteDrawData[i][2] = spriteScale;
				DDASpriteDrawData[i][3] = transform.y;
				DDASpriteDrawData[i][4] = spriteAnimIndex;
			}
		}
	}
}