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
			mapSheet_64 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/map_sheet_64.png");
			mapSheet_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/map_sheet_256_shadows.png");
			mapSheet_256->changeParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			mapSheet_256->changeParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			roof_16 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/roof.png");
			
			animFurball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/furball360_40.xml");
			animFireball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/fireball360_8.xml");
			furballShadow = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_shadow_256.png");
			animTree360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/tree360_40.xml");
			treeShadow = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/tree_shadow_512.png");

			animPlayer2d.setAnimation(animFireball360);
			animPlayer2d.setLoopStartFrame(20);
			animPlayer2d.setLoopEndFrame(29);
			animPlayer2d.loopable(true);
			animPlayer2d.start();

			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(40);
			textCreator.createTextTexture(font, "|", 255, 100, 0);
			text = textCreator.getTexture();
			textCreator3.createTextTexture(font, "|", 255, 100, 0);
			text3 = textCreator3.getTexture();

			//world size
			mapX = 24;
			mapY = 24;
			//2d tilesize
			tileSize2d = 64;
			//alpha used for rotating things
			alpha = 0.0f;

			//----------------------
			//Raycast init
			tileSize = 256;

			//How many lines for pseudo 3d
			raycastW = 800; //In pseudo3d header change Raycastlines array size accordingly
			raycastH = 800;

			camera->initCamera(0.0f, 0.0f, 800.0f, 800.0f, -2400.0f, 0.0f, 0, 0);
			//camera->setZoomLevel(2); //Use zoom if raycasting image smaller than 800. (example: raycast w = 400, zoom = 2, example1: raycast w = 200, zoom = 4) 

			moveSpeed = 0.11f;
			rotSpeed = 0.02f;
			player = { { 22.0f, 9.5f, 0.0f, 0 , 1} }; //x,y,rotation(radians), how many sides drawn, spritetype

			dirX = -1, dirY = 0; //initial direction vector
			planeX = 0.0f, planeY = 0.5; //the 2d raycaster version of camera plane

			//GameObjects.
			createFurball(15.0f, 5.0f, 0.0f);
			createFurball(12.0f, 18.3f, 0.0f);
			createFurball(13.5f, 12.7f, 0.0f);
			createFurball(14.0f, 12.0f, 0.0f);
			createFurball(15.0f, 13.8f, 0.0f);
			createFurball(16.0f, 4.0f, 0.0f);
			createFurball(18.0f, 13.0f, 0.0f);
			createFurball(12.4f, 14.9f, 0.0f);
			createFurball(23.0f, 15.0f, 0.0f);
			createFurball(12.199f, 16.2f, 0.0f);

			createFireball(5.0f, 15.0f, 0.0f);
			createFireball(7.0f, 10.0f, glm::radians(180.0f));
			createFireball(8.0f, 10.0f, 0.0f);
			createFireball(9.0f, 10.0f, glm::radians(180.0f));

			createTree(20.0f, 5.0f, 0.0f);
			createTree(20.0f, 6.0f, 0.0f);
			createTree(21.0f, 5.0f, 0.0f);
			createTree(20.0f, 7.0f, 0.0f);
			createTree(22.0f, 5.0f, 0.0f);
			createTree(20.0f, 8.0f, 0.0f);
			createTree(23.0f, 5.0f, 0.0f);
			createTree(23.0f, 8.0f, 0.0f);
			createTree(23.0f, 6.0f, 0.0f);
			createTree(21.0f, 8.0f, 0.0f);
			createTree(23.0f, 7.0f, 0.0f);
			createTree(22.0f, 8.0f, 0.0f);
			createTree(23.0f, 8.0f, 0.0f);

			createTree(20.0f, 5.0f + 6.0f, 0.0f);
			createTree(20.0f, 6.0f + 6.0f, 0.0f);
			createTree(21.0f, 5.0f + 6.0f, 0.0f);
			createTree(20.0f, 7.0f + 6.0f, 0.0f);
			createTree(22.0f, 5.0f + 6.0f, 0.0f);
			createTree(20.0f, 8.0f + 6.0f, 0.0f);
			createTree(23.0f, 5.0f + 6.0f, 0.0f);
			createTree(23.0f, 8.0f + 6.0f, 0.0f);
			createTree(23.0f, 6.0f + 6.0f, 0.0f);
			createTree(21.0f, 8.0f + 6.0f, 0.0f);
			createTree(23.0f, 7.0f + 6.0f, 0.0f);
			createTree(22.0f, 8.0f + 6.0f, 0.0f);
			createTree(23.0f, 8.0f + 6.0f, 0.0f);


#pragma endregion
			
#pragma region WallTiles

			//filling world with 0
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 25; j++)
				{
					wallTiles[i][j] = 0;
				}				
			}

			//MapSides
			for (int i = 0; i < mapY; i++)
			{
				wallTiles[0][i] = 1;
			}

			for (int i = 0; i <= mapY; i++)
			{
				wallTiles[mapX][i] = 3;
			}

			for (int i = 0; i < mapX; i++)
			{
				wallTiles[i][mapY] = 1;
			}

			for (int i = 0; i < mapX; i++)
			{
				wallTiles[i][0] = 3;
			}
			//---------------------

			for (int i = 1; i < 10; i++)
			{
				wallTiles[i][10] = 2;
			}

			for (int i = 1; i < 13; i++)
			{
				wallTiles[10][i] = 2;
			}

			for (int i = 17; i < 23; i++)
			{
				wallTiles[i][17] = 4;
			}

			for (int i = 17; i < 23; i++)
			{
				wallTiles[i][19] = 5;
			}

#pragma endregion
			
		}

		Pseudo3D::~Pseudo3D()
		{
			std::cout << "Scene shutdown" << std::endl;
		}

		void Pseudo3D::update(GLfloat step)
		{
			//Taking time it takes to go trough the update
			myTimer.start();

#pragma region RaycastMovement
			//move forward if no wall in front of you			
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_W))
			{
				if (wallTiles[int((player[0] + dirX * moveSpeed))][int(player[1])] == false) player[0] += dirX * moveSpeed;
				if (wallTiles[int(player[0])][int(player[1] + dirY * moveSpeed)] == false) player[1] += dirY * moveSpeed;
			}
			//move backwards if no wall behind you
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_S))
			{
				if (wallTiles[int(player[0] - dirX * moveSpeed)][int(player[1])] == false) player[0] -= dirX * moveSpeed;
				if (wallTiles[int(player[0])][int(player[1] - dirY * moveSpeed)] == false) player[1] -= dirY * moveSpeed;
			}
			//strafe left if no wall in left of you			
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_A))
			{
				if (wallTiles[int(player[0] - planeX * moveSpeed)][int(player[1])] == false) player[0] -= planeX * moveSpeed;
				if (wallTiles[int(player[0])][int(player[1] - planeY * moveSpeed)] == false) player[1] -= planeY * moveSpeed;
			}
			//strafe right if no wall in right of you			
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_D))
			{
				if (wallTiles[int((player[0] + planeX * moveSpeed))][int(player[1])] == false) player[0] += planeX * moveSpeed;
				if (wallTiles[int(player[0])][int(player[1] + planeY * moveSpeed)] == false) player[1] += planeY * moveSpeed;
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
			Raycasting();
			RaycastingSprites();

			//Saving player rotation
			player[2] = -glm::atan(dirX, dirY);
			if (player[2] < 0)
			{
				player[2] += glm::radians(360.0f);
			}

#pragma region Shooting
			// Shoot a projectile
			static int projectiles = 0;
			static const int projectilesMax = 5;

			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_SPACE))
			{
				createProjectile(player[0], player[1], player[2]);
			}

#pragma endregion
			
			//rotating sprites in radians
			gameObjects[5]->accessComponent<Transform>()->setRotation(alpha * 5);
			gameObjects[0]->accessComponent<Transform>()->setRotation(2*alpha);
			gameObjects[4]->accessComponent<Transform>()->setRotation(alpha + 0.02);
			gameObjects[10]->accessComponent<Transform>()->setRotation(glm::radians(315.0f) + 2*alpha); //fireball that follows the furball

			//moving sprites TODO: Make some logic and translate sprites with them.
			gameObjects[0]->accessComponent<Transform>()->setXPosition(3.0f + glm::cos(gameObjects[0]->accessComponent<Transform>()->getRotation()));
			gameObjects[0]->accessComponent<Transform>()->setYPosition(15.0f + glm::sin(gameObjects[0]->accessComponent<Transform>()->getRotation()));

			gameObjects[4]->accessComponent<Transform>()->setXPosition(15.0f + 3 * glm::cos(alpha));
			gameObjects[6]->accessComponent<Transform>()->setYPosition(10.0f + 4 * glm::sin(alpha));

			gameObjects[10]->accessComponent<Transform>()->setXPosition(3.0f + glm::cos(gameObjects[10]->accessComponent<Transform>()->getRotation()));
			gameObjects[10]->accessComponent<Transform>()->setYPosition(15.0f + glm::sin(gameObjects[10]->accessComponent<Transform>()->getRotation()));

			//moving the three fireballs
			static float firex = 24.0f;
			if (firex > 11.0f)
			{
				firex -= 0.05f;				
			}
			else
			{
				firex = 24.0f;
			}
			gameObjects[11]->accessComponent<Transform>()->setYPosition(firex);
			gameObjects[13]->accessComponent<Transform>()->setYPosition(firex);

			static float firex2 = 11.0f;
			if (firex2 < 24.0f)
			{
				firex2 += 0.05f;
			}
			else
			{
				firex2 = 11.0f;
			}
			gameObjects[12]->accessComponent<Transform>()->setYPosition(firex2);


			//2d camera
			static float zoomByInput = 1.0f;
			if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_UP))
			{
				if (zoomByInput > 0.0f)
					zoomByInput -= glm::radians(2.0f);
			}
			if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_DOWN))
			{
				zoomByInput += glm::radians(2.0f);
			}
			camera2->setZoomLevel(zoomByInput);
			camera2->setPositionRotationOrigin((player[0]*tileSize2d), (player[1]*tileSize2d));
			camera2->setRotation(player[2]);

			//Information		
			textCreator3.createTextTexture(font, "WASD + arrows " + std::to_string(player[0]) + " " + std::to_string(player[1]) + " angle: " + std::to_string(glm::degrees(player[2])), 255, 100, 0);
			text3 = textCreator3.getTexture();
			
			animPlayer2d.update();
			//gameObjects
			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->update();
			}

			//fast test for rigid
			for (int i = 10; i < gameObjects.size(); i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (gameObjects[i]->accessComponent<RigidBody>()->isColliding(gameObjects[j]))
					{
						if (gameObjects[i]->accessComponent<UserData>()->isFireball == true)
						{
							gameObjects[i]->kill();
							gameObjects[j]->accessComponent<UserData>()->isHit();
						}						
					}
				}				
			}

			// Delete dead objects
			deleteDeadObjects();
			
			//Taking time it takes to go trough the update
			myTimer.pause();
			textCreator.createTextTexture(font, "Update calculation time: " + std::to_string(myTimer.getLocalTime()) + " ms", 255, 100, 0);
			text = textCreator.getTexture();		
		}

		void Pseudo3D::interpolate(GLfloat alpha)
		{
		}

		void Pseudo3D::draw()
		{
			//Raycast draw
			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->accessComponent<PseudoSpriteDraw>()->drawPseudoSprite();
			}
						
			//Roof and floor for raycast
			opaqueBatch.draw(roof_16, &glm::vec4(0.0f, 0.0f, raycastW, raycastH), -2400.0f, raycastH / 2, raycastW, raycastH / 2, 0.0f, 0.0f, 0.0f, 1.0f, { 0.75, 0.5, 0.0 }, 1.0f, 0.0f);
			opaqueBatch.draw(roof_16, &glm::vec4(0.0f, 0.0f, raycastW, raycastH), -2400.0f, 0.0f, raycastW, raycastH / 2, 0.0f, 0.0f, 0.0f, 1.0f, { 0.4, 0.4, 0.4 }, 1.0f, 0.0f);

			//Raycast walls
			DrawRaycastLines();

			//2D camera draw
			Draw2dVision();

			//Hud
			guiBatch.draw(text, &glm::vec4(0.0f, 0.0f, text->getWidth(), text->getHeight()), camera3->getPositionRotationOrigin()[0] + glm::cos(alpha)*400.0f, camera3->getPositionRotationOrigin()[1] + 350, text->getWidth(), text->getHeight(), text->getWidth() / 2, text->getHeight() / 2, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 1.0f);
			guiBatch.draw(text3, &glm::vec4(0.0f, 0.0f, text3->getWidth(), text3->getHeight()), camera3->getPositionRotationOrigin()[0], camera3->getPositionRotationOrigin()[1] + 300, text3->getWidth(), text3->getHeight(), text3->getWidth() / 2, text3->getHeight() / 2, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 1.0f);
		}

		void Pseudo3D::Raycasting() //Vector operations are slow, changed to std::arrays, change array sizes accordingly in the Pseudo3D header.
		{
			for (int x = 0; x < raycastW; x++)
			{
				//calculate ray position and direction 
				cameraX = 2 * x / double(raycastW) - 1; //x-coordinate in camera space
				rayPosX = player[0];
				rayPosY = player[1];
				rayDirX = dirX + planeX * cameraX;
				rayDirY = dirY + planeY * cameraX;

				//which box of the map we're in  
				raycastX = int(rayPosX);
				raycastY = int(rayPosY);

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
					sideDistX = (rayPosX - raycastX) * deltaDistX;
				}
				else
				{
					stepX = 1;
					sideDistX = (raycastX + 1.0 - rayPosX) * deltaDistX;
				}
				if (rayDirY < 0)
				{
					stepY = -1;
					sideDistY = (rayPosY - raycastY) * deltaDistY;
				}
				else
				{
					stepY = 1;
					sideDistY = (raycastY + 1.0 - rayPosY) * deltaDistY;
				}

				//perform DDA
				while (hit == 0)
				{
					//jump to next map square, OR in x-direction, OR in y-direction
					if (sideDistX < sideDistY)
					{
						sideDistX += deltaDistX;
						raycastX += stepX;
						side = 0;
					}
					else
					{
						sideDistY += deltaDistY;
						raycastY += stepY;
						side = 1;
					}
					//Check if ray has hit a wall
					if (wallTiles[raycastX][raycastY] > 0)
					{
						hit = 1;
					}
				}

				//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
				if (side == 0)
					perpWallDist = fabs((raycastX - rayPosX + (1 - stepX) / 2) / rayDirX);
				else
					perpWallDist = fabs((raycastY - rayPosY + (1 - stepY) / 2) / rayDirY);

				//Calculate height of line to draw on screen
				lineHeight = glm::abs(int(raycastH / perpWallDist));

				//calculate lowest and highest pixel to fill in current stripe
				drawStart = -lineHeight / 2 + raycastH / 2;
				if (drawStart < 0)drawStart = 0;
				drawEnd = lineHeight / 2 + raycastH / 2;
				if (drawEnd >= raycastH)drawEnd = raycastH - 1;

				//texturing calculations
				texNum = wallTiles[raycastX][raycastY] - 1; //1 subtracted from it so that texture 0 can be used!

				//calculate value of wallX
				wallX; //where exactly the wall was hit
				if (side == 1) wallX = rayPosX + ((raycastY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
				else       wallX = rayPosY + ((raycastX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
				wallX -= floor((wallX));

				//x coordinate on the texture
				texX = int(wallX * double(tileSize));
				if (side == 0 && rayDirX > 0) texX = tileSize - texX - 1;
				if (side == 1 && rayDirY < 0) texX = tileSize - texX - 1;

				//choose wall color			
				switch (wallTiles[raycastX][raycastY])
				{
				case 1:  raycastTileIndex = 1;  break;
				case 2:  raycastTileIndex = 2;  break;
				case 3:  raycastTileIndex = 3;   break;
				case 4:  raycastTileIndex = 4;  break;
				default: raycastTileIndex = 5; break;
				}
				if (side == 1) { raycastTileIndex += 5; }
				
				//Saving calculated data for camera slice 
				Raycastlines[x][0] = x;
				Raycastlines[x][1] = drawStart;
				Raycastlines[x][2] = drawEnd;
				Raycastlines[x][3] = raycastTileIndex;
				Raycastlines[x][4] = texX;
				//---------------------------------------
			}			
		}

		void Pseudo3D::RaycastingSprites()
		{
			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				spriteX = gameObjects[i]->accessComponent<Transform>()->getXPosition() - player[0];
				spriteY = gameObjects[i]->accessComponent<Transform>()->getYPosition() - player[1];

				//Creating sprite transform vector
				transform = glm::vec2(spriteX, spriteY) * glm::inverse(glm::mat2x2(planeX,dirX,planeY,dirY));				

				//Calculating camera y and scale for the sprite
				spriteHeightWidth = glm::abs(raycastH / transform.y);
				spriteYout = -spriteHeightWidth / 2 + raycastH / 2;
				spriteScale = 1 / (tileSize / spriteHeightWidth);

				//Calculating x position for the sprite				
				spriteScreenX = (raycastW / 2)*(1 + transform.x / transform.y);
				spriteXout = -spriteHeightWidth / 2 + spriteScreenX;

				//Calculating sprite side				
				spriteAngle = (glm::atan(-spriteX , -spriteY));
				if (spriteAngle < 0.0f)
				{
					spriteAngle += glm::radians(360.0f);
				}

				//Sprites own facing changes the angle
				if (gameObjects[i]->accessComponent<Transform>()->getRotation() != 0.0f)
				{
					convertToFloat = float(gameObjects[i]->accessComponent<Transform>()->getRotation());
					spriteFacing = glm::mod((-convertToFloat + glm::radians(360.0f)), glm::radians(360.0f));
					spriteAngle -= spriteFacing;

					if (spriteAngle < 0.0f)
					{
						spriteAngle += glm::radians(360.0f);
					}
				}			

				spriteAnimIndex = getSpriteAnimIndex(spriteAngle, gameObjects[i]->accessComponent<UserData>()->sides);

				//Saving data
				gameObjects[i]->accessComponent<UserData>()->spriteXout = (spriteXout - 2400.0f - (gameObjects[i]->accessComponent<UserData>()->tileOverSize/2)*spriteScale);
				gameObjects[i]->accessComponent<UserData>()->spriteYout = (spriteYout);
				gameObjects[i]->accessComponent<Transform>()->setScale(spriteScale);
				gameObjects[i]->accessComponent<UserData>()->transformY = transform.y;

				if (gameObjects[i]->accessComponent<UserData>()->isFireball == true)
				{
					gameObjects[i]->accessComponent<AnimationPlayer>()->setLoopStartFrame(int(spriteAnimIndex * 10));
					gameObjects[i]->accessComponent<AnimationPlayer>()->setLoopEndFrame(int(spriteAnimIndex * 10 + 9));
				}
				//Is the object hit
				else if (gameObjects[i]->accessComponent<UserData>()->hitCoolDown.isStarted() == true)
				{
					//Is the cooldown gone
					if (gameObjects[i]->accessComponent<UserData>()->hitCoolDown.getLocalTime() > 2000)
					{
						gameObjects[i]->accessComponent<UserData>()->hitCoolDown.stop();
						gameObjects[i]->accessComponent<AnimationPlayer>()->setCurrentFrame(spriteAnimIndex);
					}					
				}
				//If not hit then change index
				else
				{
					gameObjects[i]->accessComponent<AnimationPlayer>()->setCurrentFrame(spriteAnimIndex);
				}

				//Scaling for the sprite
				if (spriteYout <= 0)
				{
					depth = 2.0; //disappear
				}
				else
				{
					depth = (1.0f / spriteYout);
				}
				gameObjects[i]->accessComponent<Transform>()->setDepth(depth);
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

		void Pseudo3D::DrawRaycastLines()
		{
			for (int i = 0; i < Raycastlines.size(); i++)
			{
				if (Raycastlines[i][2] > 0)
				{
					if (Raycastlines[i][1] <= 0)
					{
						depth = 0.99f;
					}
					else
					{
						depth = (1.0f / Raycastlines[i][1]);
					}

					opaqueBatch.draw(mapSheet_256, &glm::vec4(Raycastlines[i][4] + (int(Raycastlines[i][3]) - 1) * tileSize, 0.0f, 1.0f, tileSize), Raycastlines[i][0] - 2400, Raycastlines[i][1], 1.0f, Raycastlines[i][2] - Raycastlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, depth);
				}
			}
		}

		void Pseudo3D::Draw2dVision()
		{
			//walls.
			for (int i = 0; i <= mapY; i++)
			{
				for (int j = 0; j <= mapX; j++)
				{
					float offset = 32.0f;
					if (wallTiles[j][i] != 0)
					{
						opaqueBatch.draw(mapSheet_64, &glm::vec4((int(wallTiles[j][i]) - 1) * 66, 1.0f, 64, 64), (j * tileSize2d) + offset, i * tileSize2d + offset, 64.0f, 64.0f, 32.0f, 32.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 0.1f);
					}
				}
			}
			//sprites
			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				if (gameObjects[i]->accessComponent<UserData>()->isFireball != true)
				{
					alphaBatch.draw(furball, &glm::vec4(0.0f, 0.0f, furball->getWidth(), furball->getHeight()),
						gameObjects[i]->accessComponent<Transform>()->getXPosition() * tileSize2d, gameObjects[i]->accessComponent<Transform>()->getYPosition() * tileSize2d, furball->getWidth(),
						furball->getHeight(), tileSize2d / 2, tileSize2d / 2, gameObjects[i]->accessComponent<Transform>()->getRotation(), 1.0f, Renderer::clrWhite, 1.0f, 0.7f + i*0.000001f);
				}
				//if fireball
				else
				{
					alphaBatch.draw(animPlayer2d.getTexture(), animPlayer2d.getCurrentFrameTexCoords(),
						gameObjects[i]->accessComponent<Transform>()->getXPosition() * tileSize2d, gameObjects[i]->accessComponent<Transform>()->getYPosition() * tileSize2d, 256, 256, 256 / 2, 256 / 2, gameObjects[i]->accessComponent<Transform>()->getRotation() + glm::radians(90.0f),
						0.25f, Renderer::clrWhite, 1.0f, 0.8f + i * 0.001f);
				}
			}

			//player
			alphaBatch.draw(furball, &glm::vec4(0.0f, 0.0f, furball->getWidth(), furball->getHeight()), player[0] * tileSize2d, player[1] * tileSize2d, furball->getWidth(), furball->getHeight(), tileSize2d / 2, tileSize2d / 2, player[2], 1.0f, Renderer::clrRed, 1.0f, 0.8f);

			//2d floor
			opaqueBatch.draw(roof_16, &glm::vec4(0.0f, 0.0f, mapX * tileSize2d, mapY * tileSize2d), 0.0f, 0.0f, mapX * tileSize2d, mapY * tileSize2d, 0.0f, 0.0f, 0.0f, 1.0f, { 0.5, 0.5, 0.5 }, 1.0f, 0.0f);
			//---------------
		}

		//Raycast furball sprite
		void Pseudo3D::createFurball(float x, float y, float rotation)
		{
			gameObjects.push_back(new GameObject(&alphaBatch));
			gameObjects.back()->addComponent<Transform>();
			gameObjects.back()->addComponent<RigidBody>();
			gameObjects.back()->addComponent<Sprite>();
			gameObjects.back()->addComponent<AnimationPlayer>();
			gameObjects.back()->addComponent<UserData>();
			gameObjects.back()->addComponent<PseudoSpriteDraw>();

			gameObjects.back()->accessComponent<AnimationPlayer>()->setAnimation(animFurball360);
			gameObjects.back()->accessComponent<AnimationPlayer>()->setLoopEndFrame(0);
			gameObjects.back()->accessComponent<AnimationPlayer>()->loopable(true);
			gameObjects.back()->accessComponent<AnimationPlayer>()->pause();
			gameObjects.back()->accessComponent<AnimationPlayer>()->setCurrentFrame(0);			
			gameObjects.back()->accessComponent<Transform>()->setXPosition(x);
			gameObjects.back()->accessComponent<Transform>()->setYPosition(y);
			gameObjects.back()->accessComponent<Transform>()->setRotation(rotation);

			//userdata				
			gameObjects.back()->accessComponent<UserData>()->sides = 40;
			gameObjects.back()->accessComponent<Transform>()->setDepth(1.0f);
			gameObjects.back()->accessComponent<UserData>()->transformY = 0.0f;
			gameObjects.back()->accessComponent<UserData>()->animationIndex = 0;
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setTextureBatch(&alphaBatch);
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setRaycastW(raycastW);
			gameObjects.back()->accessComponent<UserData>()->shadow = furballShadow;
			gameObjects.back()->accessComponent<UserData>()->hasShadow = true;
			gameObjects.back()->accessComponent<UserData>()->hitAnimStart = 40;
			gameObjects.back()->accessComponent<UserData>()->hitAnimEnd = 63;

			//rigidBody
			gameObjects.back()->accessComponent<RigidBody>()->setCollisionRadius(0.4f);
		}

		//Raycast fireball sprite
		void Pseudo3D::createFireball(float x, float y, float rotation)
		{
			gameObjects.push_back(new GameObject(&alphaBatch));
			gameObjects.back()->addComponent<Transform>();
			gameObjects.back()->addComponent<RigidBody>();
			gameObjects.back()->addComponent<Sprite>();
			gameObjects.back()->addComponent<AnimationPlayer>();
			gameObjects.back()->addComponent<UserData>();
			gameObjects.back()->addComponent<PseudoSpriteDraw>();

			gameObjects.back()->accessComponent<AnimationPlayer>()->setAnimation(animFireball360);
			gameObjects.back()->accessComponent<AnimationPlayer>()->setLoopEndFrame(9);
			gameObjects.back()->accessComponent<AnimationPlayer>()->loopable(true);
			gameObjects.back()->accessComponent<AnimationPlayer>()->start();
			
			gameObjects.back()->accessComponent<Transform>()->setXPosition(x);
			gameObjects.back()->accessComponent<Transform>()->setYPosition(y);
			gameObjects.back()->accessComponent<Transform>()->setRotation(rotation);

			//userdata
			gameObjects.back()->accessComponent<UserData>()->sides = 8;
			gameObjects.back()->accessComponent<Transform>()->setDepth(1.0f);
			gameObjects.back()->accessComponent<UserData>()->transformY = 0.0f;
			gameObjects.back()->accessComponent<UserData>()->animationIndex = 0;
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setTextureBatch(&alphaBatch);
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setRaycastW(raycastW);

			gameObjects.back()->accessComponent<UserData>()->isFireball = true;
			gameObjects.back()->accessComponent<UserData>()->hasShadow = false;
		}

		//Raycast tree sprite
		void Pseudo3D::createTree(float x, float y, float rotation)
		{
			gameObjects.push_back(new GameObject(&alphaBatch));
			gameObjects.back()->addComponent<Transform>();
			gameObjects.back()->addComponent<RigidBody>();
			gameObjects.back()->addComponent<Sprite>();
			gameObjects.back()->addComponent<AnimationPlayer>();
			gameObjects.back()->addComponent<UserData>();
			gameObjects.back()->addComponent<PseudoSpriteDraw>();

			gameObjects.back()->accessComponent<AnimationPlayer>()->setAnimation(animTree360);
			gameObjects.back()->accessComponent<AnimationPlayer>()->setLoopEndFrame(0);
			gameObjects.back()->accessComponent<AnimationPlayer>()->loopable(true);
			gameObjects.back()->accessComponent<AnimationPlayer>()->pause();
			gameObjects.back()->accessComponent<AnimationPlayer>()->setCurrentFrame(0);
			gameObjects.back()->accessComponent<Transform>()->setXPosition(x);
			gameObjects.back()->accessComponent<Transform>()->setYPosition(y);
			gameObjects.back()->accessComponent<Transform>()->setRotation(rotation);

			//userdata				
			gameObjects.back()->accessComponent<UserData>()->sides = 40;
			gameObjects.back()->accessComponent<Transform>()->setDepth(1.0f);
			gameObjects.back()->accessComponent<UserData>()->transformY = 0.0f;
			gameObjects.back()->accessComponent<UserData>()->animationIndex = 0;
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setTextureBatch(&alphaBatch);
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setRaycastW(raycastW);
			gameObjects.back()->accessComponent<UserData>()->tileOverSize = 256;

			gameObjects.back()->accessComponent<UserData>()->shadow = treeShadow;
			gameObjects.back()->accessComponent<UserData>()->hasShadow = true;
		}

		//Raycast fireball projectile
		void Pseudo3D::createProjectile(float x, float y, float rotation)
		{
			gameObjects.push_back(new GameObject(&alphaBatch));
			gameObjects.back()->addComponent<Transform>();
			gameObjects.back()->addComponent<RigidBody>();
			gameObjects.back()->addComponent<Sprite>();
			gameObjects.back()->addComponent<AnimationPlayer>();
			gameObjects.back()->addComponent<UserData>();
			gameObjects.back()->addComponent<PseudoSpriteDraw>();
			gameObjects.back()->addComponent<Projectile>();

			gameObjects.back()->accessComponent<AnimationPlayer>()->setAnimation(animFireball360);
			gameObjects.back()->accessComponent<AnimationPlayer>()->setLoopEndFrame(9);
			gameObjects.back()->accessComponent<AnimationPlayer>()->loopable(true);
			gameObjects.back()->accessComponent<AnimationPlayer>()->start();

			gameObjects.back()->accessComponent<Transform>()->setXPosition(x);
			gameObjects.back()->accessComponent<Transform>()->setYPosition(y);
			gameObjects.back()->accessComponent<Transform>()->setRotation(rotation);

			//userdata
			gameObjects.back()->accessComponent<UserData>()->sides = 8;
			gameObjects.back()->accessComponent<Transform>()->setDepth(1.0f);
			gameObjects.back()->accessComponent<UserData>()->transformY = 0.0f;
			gameObjects.back()->accessComponent<UserData>()->animationIndex = 0;
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setTextureBatch(&alphaBatch);
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setRaycastW(raycastW);

			gameObjects.back()->accessComponent<UserData>()->isFireball = true;

			//rigidBody
			gameObjects.back()->accessComponent<RigidBody>()->setCollisionRadius(0.2f);
		}

		void Pseudo3D::Projectile::update()
		{
			Transform* t = ownerObject->accessComponent<Transform>();
			t->setXPosition(t->getXPosition() + speed * cosf(t->getRotation() + glm::radians(90.0f)));
			t->setYPosition(t->getYPosition() + speed * sinf(t->getRotation() + glm::radians(90.0f)));

			if (t->getXPosition() < 1.0f || t->getXPosition() > 24.0f || t->getYPosition() < 1.0f || t->getYPosition() > 24.0f)
			{
				ownerObject->kill();
			}
		}

		void Pseudo3D::deleteDeadObjects()
		{
			gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(), [&](GameObject *obj)
			{ 
				if (!obj->isAlive())
				{
					deadObjects.push_back(obj);
					return true;
				}
				return false;
			}), gameObjects.end());

			for (auto deadObject : deadObjects)
			{
				delete deadObject;
			}

			deadObjects.clear();
		}
	}
}