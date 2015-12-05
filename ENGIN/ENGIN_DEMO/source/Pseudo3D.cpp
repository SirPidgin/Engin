#include "Pseudo3D.h"

#include <iostream>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Color.h>
#include "Engin\Core\RNG.h"
#include <glm\gtc\type_ptr.hpp>

namespace Engin
{
	namespace Game
	{
		Pseudo3D::Pseudo3D(Engin* engine) : camera(createWorldCamera()), camera2(createWorldCamera()), camera3(createGuiCamera()), useGamePad(false)
		{
#pragma region INIT
			this->engine = engine;

			std::cout << "Scene started" << std::endl;

			if (engine->gamepadInput->getNumGamepads() > 0)
			{
				useGamePad = true;
			}

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
			furball_128 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_upside_128.png");
			mapSheet_64 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/map_sheet_64.png");
			mapSheet_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/map_sheet_256_shadows.png");
			mapSheet_256->changeParameters(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			mapSheet_256->changeParameters(GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			floor_16 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/roof.png");
			floor_800 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/floor.png");
			
			animFurball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/furball360_40.xml");
			animFireball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/fireball360_8.xml");
			furballShadow = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_shadow_256.png");
			animTree360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/tree360_40.xml");
			treeShadow = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/tree_shadow_512.png");
			tree_64 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/tree_up_64.png");

			animFurballHit = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/fireball_hit_36.xml");

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

			// Player Movement
			moveSpeed = 0.08f;
			rotSpeed = 0.03f;
			player = { { 22.0f, 9.5f, 0.0f, 0 , 1} }; //x,y,rotation(radians, used only in GUI currently), how many sides drawn, spritetype.
			dirX = 1.0, dirY = 0; // Initial direction vector
			planeX = 0.0f, planeY = -0.5; // The 2d raycaster version of camera plane

			//GameObjects.
			createFurball(15.0f, 5.0f, 0.0f);
			createFurball(8.0f, 18.3f, 0.0f);
			createFurball(13.5f, 12.7f, 0.0f); // Player 2 currently
			gameObjects.back()->accessComponent<UserData>()->spriteColorR = 0.5f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorG = 0.5f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorB = 5.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorA = 0.8f;

			createFurball(14.0f, 12.0f, 0.0f);
			createFurball(15.0f, 13.8f, 0.0f);
			createFurball(16.0f, 4.0f, 0.0f);
			createFurball(18.0f, 13.0f, 0.0f);
			createFurball(12.4f, 14.9f, 0.0f);
			createFurball(23.0f, 15.0f, 0.0f);
			createFurball(12.199f, 16.2f, 0.0f);

			createTree(0.0f, 0.0f, 0.0f); //furball drag

			createTree(20.0f, 5.0f, 0.0f);
			createTree(20.0f, 6.0f, 0.0f);
			createTree(20.0f, 7.0f, 0.0f);
			createTree(20.0f, 8.0f, 0.0f);
			createTree(21.0f, 5.0f, 0.0f);
			createTree(21.0f, 8.0f, 0.0f);			
			createTree(22.0f, 5.0f, 0.0f);
			createTree(22.0f, 8.0f, 0.0f);
			createTree(23.0f, 5.0f, 0.0f);			
			createTree(23.0f, 6.0f, 0.0f);			
			createTree(23.0f, 7.0f, 0.0f);
			createTree(23.0f, 8.0f, 0.0f);
			
			createTree(20.0f, 5.0f+ 6.0f, 0.0f);
			createTree(20.0f, 6.0f+ 6.0f, 0.0f);
			createTree(20.0f, 7.0f+ 6.0f, 0.0f);
			createTree(20.0f, 8.0f+ 6.0f, 0.0f);
			createTree(21.0f, 5.0f+ 6.0f, 0.0f);
			createTree(21.0f, 8.0f+ 6.0f, 0.0f);
			createTree(22.0f, 5.0f+ 6.0f, 0.0f);
			createTree(22.0f, 8.0f+ 6.0f, 0.0f);
			createTree(23.0f, 5.0f+ 6.0f, 0.0f);
			createTree(23.0f, 6.0f+ 6.0f, 0.0f);
			createTree(23.0f, 7.0f+ 6.0f, 0.0f);
			createTree(23.0f, 8.0f+ 6.0f, 0.0f);
			
			turretCoolDown.start();


#pragma endregion

#pragma region WallTiles

			// World size, Remember to change the world array size in header.
			mapX = 24;
			mapY = 48;			

			// Tilemap: North up. South down. East right. West left. Remember that degrees 0 is towards east;
			wallTiles = 
			{ {
				{ { 4, 4, 4, 4, 4, 4, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
				{ { 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 3, 3, 3, 0, 1, 0, 0, 0, 0, 3 } },
				{ { 4, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 2, 0, 0, 3, 0, 3, 0, 1, 0, 0, 0, 0, 3 } },
				{ { 4, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 3, 0, 3, 0, 1, 0, 0, 0, 0, 3 } },
				{ { 4, 1, 4, 1, 4, 1, 4, 0, 0, 0, 0, 0, 2, 2, 0, 3, 3, 3, 0, 1, 1, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 2, 0, 2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5 } },
				{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 } },
				{ { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 1, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 } },
				{ { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 } },
			} };

#pragma endregion
			
		}

		Pseudo3D::~Pseudo3D()
		{
			std::cout << "Scene shutdown" << std::endl;
		}

		// Moves player
		void Pseudo3D::movePlayer(float multiplier)
		{
			if (wallTiles[mapY - static_cast<int>(player[1])][static_cast<int>(player[0] + dirX * moveSpeed * multiplier)] == false)
			{
				player[0] += dirX * moveSpeed * multiplier;
			}

			if (wallTiles[mapY - static_cast<int>(player[1] + dirY * moveSpeed * multiplier)][static_cast<int>(player[0])] == false)
			{
				player[1] += dirY * moveSpeed * multiplier;
			}
		}

		// Strafes player.
		void Pseudo3D::strafePlayer(float multiplier)
		{
			if (wallTiles[mapY - static_cast<int>(player[1])][static_cast<int>(player[0] + planeX * moveSpeed * multiplier)] == false)
			{
				player[0] += planeX * moveSpeed * multiplier;
			}

			if (wallTiles[mapY - static_cast<int>(player[1] + planeY * moveSpeed * multiplier)][static_cast<int>(player[0])] == false)
			{
				player[1] += planeY * moveSpeed * multiplier;
			}
		}

		// Rotates player.
		void Pseudo3D::rotatePlayer(float speed)
		{
			// Both camera direction and camera plane must be rotated.
			double oldDirX = dirX;
			dirX = dirX * cos(speed) - dirY * sin(speed);
			dirY = oldDirX * sin(speed) + dirY * cos(speed);
			double oldPlaneX = planeX;
			planeX = planeX * cos(speed) - planeY * sin(speed);
			planeY = oldPlaneX * sin(speed) + planeY * cos(speed);
		}

		// Get requested axis multiplier of given gamepad.
		float Pseudo3D::getAxisMultiplier(HID::GamepadAxis axis, int GPIndex)
		{
			return engine->gamepadInput->getAxisValue(axis, GPIndex) / HID::AXIS_MAX;
		}

		void Pseudo3D::update(GLfloat step)
		{
			// Taking time it takes to go trough the update.
			myTimer.start();

#pragma region RaycastMovement

			// Player movement.
			float axisMultiplier = 0.0f;
			float deadzone = 0.175f;

			// Move forward if no wall in front of you.
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_W))
			{
				movePlayer(1.0f);
			}
			// Move backwards if no wall behind you.
			else if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_S))
			{
				movePlayer(-1.0f);
			}
			// Check pad movement.
			else if (useGamePad)
			{
				axisMultiplier = -getAxisMultiplier(HID::GAMEPAD_AXIS_LEFTY, 0);

				if (axisMultiplier < -deadzone || axisMultiplier > deadzone)
				{
					movePlayer(axisMultiplier);
				}

			}

			// Strafe left.
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_A))
			{
				strafePlayer(-1.0f);
			}
			// Strafe right.
			else if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_D))
			{
				strafePlayer(1.0f);
			}
			// Check pad movement.
			else if (useGamePad)
			{
				axisMultiplier = getAxisMultiplier(HID::GAMEPAD_AXIS_LEFTX, 0);

				if (axisMultiplier < -deadzone || axisMultiplier > deadzone)
				{
					strafePlayer(axisMultiplier);
				}
			}

			//Mouse rotation
			static int lastMouseX = 0;
			static int lastMouseY = 0;
			static int currMouseX = 0;
			static int currMouseY = 0;
			static float realRotSpeed = rotSpeed;
			static float mouseSens = 300.0f;
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_PAGEUP))
			{
				mouseSens -= 5.0f;
			}
			else if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_PAGEDOWN))
			{
				mouseSens += 5.0f;
			}
			if (mouseSens <= 0.0f)
			{
				mouseSens = 1.0f;
			}
			engine->mouseInput->getRelativeMouseState(&currMouseX, &currMouseY);
			realRotSpeed = rotSpeed + (float)(abs(currMouseX) - abs(lastMouseX));
			realRotSpeed /= mouseSens;
			// Keyboard rotate to the right.
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_RIGHT))
			{
				rotatePlayer(rotSpeed * -1.0f);
			}
			// Keyboard rotate to the left.
			else if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_LEFT))
			{
				rotatePlayer(rotSpeed);
			}
			// Mouse rotate to the right.
			else if (lastMouseX < currMouseX)
			{
				rotatePlayer(-realRotSpeed);
			}
			// Mouse rrotate to the left.
			else if (lastMouseX > currMouseX)
			{
				rotatePlayer(realRotSpeed);
			}
			// Check pad movement.
			else if (useGamePad)
			{
				axisMultiplier = getAxisMultiplier(HID::GAMEPAD_AXIS_RIGHTX, 0);

				if (axisMultiplier < -deadzone || axisMultiplier > deadzone)
				{
					rotatePlayer(-axisMultiplier*rotSpeed);
				}
			}

			engine->mouseInput->getRelativeMouseState(&lastMouseX, &lastMouseY);
#pragma endregion

			alpha += 0.01;			

			// Saving player rotation.
			player[2] = glm::atan(dirY, dirX);
			if (player[2] < 0)
			{
				player[2] += glm::radians(360.0f);
			}

#pragma region Shooting
			// Shoot a projectile.
			static Core::Timer shootTimer;

			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_SPACE))
			{
				createProjectile(player[0], player[1], player[2]);
			}
			else if (useGamePad)
			{
				axisMultiplier = getAxisMultiplier(HID::GAMEPAD_AXIS_TRIGGERRIGHT, 0);

				if (axisMultiplier > 0.0f && (!shootTimer.isStarted() || shootTimer.getLocalTime() > (1.0f - axisMultiplier + 0.1f) * 1000.0f))
				{

				createProjectile(player[0], player[1], player[2]);
					shootTimer.start();
				}
			}

#pragma endregion
			
			// Rotating sprites in radians.
			gameObjects[5]->accessComponent<Transform>()->setRotation(alpha * 5);
			gameObjects[0]->accessComponent<Transform>()->setRotation(2*alpha);
			gameObjects[4]->accessComponent<Transform>()->setRotation(alpha + 0.02);
			gameObjects[10]->accessComponent<Transform>()->setRotation(glm::radians(315.0f) + 2*alpha);

			gameObjects[1]->accessComponent<Transform>()->setRotation(2 * alpha);

			// Moving sprites TODO: Make some logic and translate sprites with them.
			gameObjects[0]->accessComponent<Transform>()->setXPosition(3.0f + glm::cos(gameObjects[0]->accessComponent<Transform>()->getRotation() - glm::radians(90.0f)));
			gameObjects[0]->accessComponent<Transform>()->setYPosition(15.0f + glm::sin(gameObjects[0]->accessComponent<Transform>()->getRotation() - glm::radians(90.0f)));

			gameObjects[4]->accessComponent<Transform>()->setXPosition(15.0f + 3 * glm::cos(alpha));
			gameObjects[6]->accessComponent<Transform>()->setYPosition(10.0f + 4 * glm::sin(alpha));

			gameObjects[10]->accessComponent<Transform>()->setXPosition(3.0f + glm::cos(gameObjects[10]->accessComponent<Transform>()->getRotation() - glm::radians(90.0f)));
			gameObjects[10]->accessComponent<Transform>()->setYPosition(15.0f + glm::sin(gameObjects[10]->accessComponent<Transform>()->getRotation() - glm::radians(90.0f)));

			gameObjects[1]->accessComponent<Transform>()->setXPosition(8.0f + 4.0f * glm::cos(alpha));

#pragma region Launchers
			// Fireball launchers	

			if (turretCoolDown.getLocalTime() > 2500.0f)
			{
				createProjectile(6, 11, glm::radians(90.0f));
				createProjectile(8, 11, glm::radians(90.0f));
				createProjectile(7, 24, glm::radians(270.0f));
				turretCoolDown.start();
			}
#pragma endregion

#pragma region Player2
				{ // Moving player 2. TODO change player 1 to gameobject...
					float multiplier = 0.0f;

					// Player 2 strafe.
					if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_KP_7) || engine->keyboardInput->keyIsPressed(HID::KEYBOARD_KP_9));
					{
				
					}

					// Move forwards or backwards.
					if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_KP_8) || engine->keyboardInput->keyIsPressed(HID::KEYBOARD_KP_5));
					{
						float rotX = glm::cos(gameObjects[2]->accessComponent<Transform>()->getRotation());
						float rotY = glm::sin(gameObjects[2]->accessComponent<Transform>()->getRotation());

						if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_KP_8))
						{
							multiplier = 1.0f;
						}
						if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_KP_5))
						{
							multiplier = -1.0f;
						}

						if (wallTiles[mapY - static_cast<int>(gameObjects[2]->accessComponent<Transform>()->getYPosition())][static_cast<int>(gameObjects[2]->accessComponent<Transform>()->getXPosition() + rotX * moveSpeed* multiplier)] == false)
						{
							gameObjects[2]->accessComponent<Transform>()->setXPosition(gameObjects[2]->accessComponent<Transform>()->getXPosition() + rotX * moveSpeed * multiplier);
						}
						if (wallTiles[mapY - static_cast<int>(gameObjects[2]->accessComponent<Transform>()->getYPosition() + rotY * moveSpeed * multiplier)][static_cast<int>(gameObjects[2]->accessComponent<Transform>()->getXPosition())] == false)
						{
							gameObjects[2]->accessComponent<Transform>()->setYPosition(gameObjects[2]->accessComponent<Transform>()->getYPosition() + rotY * moveSpeed * multiplier);
						}						
					}				

					// Player 2 rotation.
					if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_KP_4))
					{
						gameObjects[2]->accessComponent<Transform>()->setRotation(gameObjects[2]->accessComponent<Transform>()->getRotation() + rotSpeed);
					}
					if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_KP_6))
					{
						gameObjects[2]->accessComponent<Transform>()->setRotation(gameObjects[2]->accessComponent<Transform>()->getRotation() - rotSpeed);
					}
				}

				// Player 2 camera zoom
				static float zoomByInput = 1.0f;
				if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_UP))
				{
					if (zoomByInput > 0.0f)
					{
						zoomByInput -= glm::radians(2.0f);
					}
				}
				if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_DOWN))
				{
					zoomByInput += glm::radians(2.0f);
				}

				// Camera for player 2.
				camera2->setZoomLevel(zoomByInput);
				camera2->setPositionRotationOrigin(gameObjects[2]->accessComponent<Transform>()->getXPosition()*tileSize2d, gameObjects[2]->accessComponent<Transform>()->getYPosition()*tileSize2d);
				camera2->setRotation(gameObjects[2]->accessComponent<Transform>()->getRotation() - glm::radians(90.0f));
#pragma endregion


			// Information display.		
			textCreator3.createTextTexture(font, "WASD + arrows " + std::to_string(player[0]) + " " + std::to_string(player[1]) + " angle: " + std::to_string(glm::degrees(player[2])), 255, 100, 0);
			text3 = textCreator3.getTexture();
			
			// 2D animation player for 2D fireballs.
			animPlayer2d.update();
			
			// GameObjects.
			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->update();

				// Kills the fireball if it hits a wall.
				if (gameObjects[i]->accessComponent<UserData>()->isFireball)
				{
					Transform* t = gameObjects[i]->accessComponent<Transform>();

					if (wallTiles[mapY - static_cast<int>(t->getYPosition())][static_cast<int>(t->getXPosition())] != false)
					{
						gameObjects[i]->kill();
					}
				}

				if (gameObjects[i]->accessComponent<UserData>()->isHitAnimation)
				{
					gameObjects[i]->accessComponent<UserData>()->hitAnimationUpdate();
				}

				// Kill hit animations
				if (gameObjects[i]->accessComponent<UserData>()->hitCoolDown.getLocalTime() > gameObjects[i]->accessComponent<UserData>()->cooldownLenght)
				{
 					gameObjects[i]->kill();
				}				
			}

			// Collision test for rigid.
			for (size_t i = 10; i < gameObjects.size(); i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (gameObjects[i]->accessComponent<RigidBody>()->isColliding(gameObjects[j]))
					{
						if (gameObjects[i]->accessComponent<UserData>()->isFireball == true)
						{
							// Kill the projectile.
							gameObjects[i]->kill(); 
							// Create hit animation.
							createHitFireball(gameObjects[j]->accessComponent<Transform>()->getXPosition(), gameObjects[j]->accessComponent<Transform>()->getYPosition(), gameObjects[j]->accessComponent<Transform>());
							// Color the furball to look like ghost.							
							gameObjects[j]->accessComponent<UserData>()->spriteColorR = 0.5f;
							gameObjects[j]->accessComponent<UserData>()->spriteColorG = 0.5f;
							gameObjects[j]->accessComponent<UserData>()->spriteColorB = 5.0f;
							gameObjects[j]->accessComponent<UserData>()->spriteColorA = 0.8f;
						}						
					}
				}				
			}

			// Delete dead objects
			deleteDeadObjects();

			// Raycast calculations.
			Raycasting();
			RaycastingSprites();
			
			// Taking time it takes to go trough the update
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
			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->accessComponent<PseudoSpriteDraw>()->drawPseudoSprite();
			}
						
			//Roof and floor for raycast
			opaqueBatch.draw(floor_800, &glm::vec4(0.0f, 0.0f, floor_800->getWidth(), floor_800->getHeight()), -2400.0f, 400.0f, floor_800->getWidth(), floor_800->getHeight(), 0.0f, 0.0f, 0.0f, 1.0f, Renderer::Color{ 0.5, 0.2, 0.0 } * 2.0f, 1.0f, 0.0f);
			opaqueBatch.draw(floor_800, &glm::vec4(0.0f, 0.0f, floor_800->getWidth(), floor_800->getHeight()), -2400.0f, 0.0f, floor_800->getWidth(), floor_800->getHeight(), 0.0f, 0.0f, 0.0f, 1.0f, Renderer::Color{ 0.3, 0.3, 0.4 } * 3.0f, 1.0f, 0.0f);

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
				cameraX = 2 * x / static_cast<double>(raycastW)-1; //x-coordinate in camera space
				rayPosX = player[0];
				rayPosY = player[1];
				rayDirX = dirX + planeX * cameraX;
				rayDirY = dirY + planeY * cameraX;

				//which box of the map we're in  
				raycastX = static_cast<int>(rayPosX);
				raycastY = static_cast<int>(rayPosY);

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
					if (wallTiles[mapY - raycastY][raycastX] > 0)
					{
						hit = 1;
					}
				}

				//Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
				if (side == 0)
				{
					perpWallDist = fabs((raycastX - rayPosX + (1 - stepX) / 2) / rayDirX);
				}	
				else
				{
					perpWallDist = fabs((raycastY - rayPosY + (1 - stepY) / 2) / rayDirY);
				}

				//Calculate height of line to draw on screen
				lineHeight = glm::abs(static_cast<int>(raycastH / perpWallDist));

				//calculate lowest and highest pixel to fill in current stripe
				drawStart = -lineHeight / 2 + raycastH / 2;
				if (drawStart < 0)
				{
					drawStart = 0;
				}

				drawEnd = lineHeight / 2 + raycastH / 2;
				if (drawEnd >= raycastH)
				{
					drawEnd = raycastH - 1;
				}

				//texturing calculations
				texNum = wallTiles[mapY - raycastY][raycastX] - 1; //1 subtracted from it so that texture 0 can be used!

				//calculate value of wallX
				wallX; //where exactly the wall was hit
				if (side == 1)
				{
					wallX = rayPosX + ((raycastY - rayPosY + (1 - stepY) / 2) / rayDirY) * rayDirX;
				}
				else
				{
					wallX = rayPosY + ((raycastX - rayPosX + (1 - stepX) / 2) / rayDirX) * rayDirY;
				}

				wallX -= floor((wallX));

				//x coordinate on the texture
				texX = static_cast<int>(wallX * static_cast<double>(tileSize));
				if (side == 0 && rayDirX > 0)
				{
					texX = tileSize - texX - 1;
				}
				else if (side == 1 && rayDirY < 0)
				{
					texX = tileSize - texX - 1;
				}

				//choose wall color			
				switch (wallTiles[mapY - raycastY][raycastX])
				{
				case 1:  raycastTileIndex = 1;  break;
				case 2:  raycastTileIndex = 2;  break;
				case 3:  raycastTileIndex = 3;   break;
				case 4:  raycastTileIndex = 4;  break;
				default: raycastTileIndex = 5; break;
				}

				if (side == 1) 
				{ 
					raycastTileIndex += 5; 
				}
				
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
				spriteAngle = (glm::atan(-spriteX, -spriteY)) - glm::radians(90.0f);  // TODO: Fix rotation 90 degrees differently.
				if (spriteAngle < 0.0f)
				{
					spriteAngle += glm::radians(360.0f);
				}

				//Sprites own facing changes the angle
				if (gameObjects[i]->accessComponent<Transform>()->getRotation() != 0.0f)
				{
					convertToFloat = static_cast<float>(gameObjects[i]->accessComponent<Transform>()->getRotation());
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

				//TODO: Animations need better system for selecting indexes.
				if (gameObjects[i]->accessComponent<UserData>()->isFireball == true)
				{
					gameObjects[i]->accessComponent<AnimationPlayer>()->setLoopStartFrame(static_cast<int>(spriteAnimIndex * 10));
					gameObjects[i]->accessComponent<AnimationPlayer>()->setLoopEndFrame(static_cast<int>(spriteAnimIndex * 10 + 9));
				}
				else if (gameObjects[i]->accessComponent<UserData>()->isHitAnimation == true)
				{
					//Do nothing
				}
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
					depth = 1.0f - (spriteYout / (raycastH/2));
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
				return static_cast<int>((glm::degrees(angle) + (spriteSideAngle / 2.0f)) / spriteSideAngle);
			}
		}

		void Pseudo3D::DrawRaycastLines()
		{
			for (size_t i = 0; i < Raycastlines.size(); i++)
			{
				if (Raycastlines[i][2] > 0)
				{
					if (Raycastlines[i][1] <= 0)
					{
						depth = 0.99f;
					}
					else
					{
						depth = 1.0f - (Raycastlines[i][1] / (raycastH / 2));
					}
					if (depth > 1.0f)
					{
						depth = 1.0f;
					}

					static float colorValue = 0;
					colorValue = depth * 6;
					if (colorValue > 0.8f)
					{
						colorValue = 0.8f;
					}
					else if (colorValue < 0.4f)
					{
						colorValue = 0.4f;
					}

					opaqueBatch.draw(mapSheet_256, &glm::vec4(Raycastlines[i][4] + (static_cast<int>(Raycastlines[i][3]) - 1) * tileSize, 0.0f, 1.0f, tileSize), 
						Raycastlines[i][0] - 2400, Raycastlines[i][1], 1.0f, Raycastlines[i][2] - Raycastlines[i][1], 0.0f, 0.0f, 0.0f, 1.0f, 
						Renderer::Color{ 1.0f, 1.0f, 1.0f } * colorValue, 1.0f, depth);
				}
			}
		}

		void Pseudo3D::Draw2dVision()
		{
			//walls.
			for (int i = mapY; i >= 0; i--)
			{
				for (int j = 0; j <= mapX; j++)
				{
					float offset = 32.0f;
					if (wallTiles[i][j] != 0)
					{
						opaqueBatch.draw(mapSheet_64, &glm::vec4((static_cast<int>(wallTiles[i][j]) - 1) * 66, 1.0f, 64, 64), (j * tileSize2d) + offset, ((mapY - i) * tileSize2d) + offset, 64.0f, 64.0f, 32.0f, 32.0f, 0.0f, 1.0f, Renderer::clrWhite, 1.0f, 0.1f);
					}
				}
			}
			//sprites
			for (size_t i = 0; i < gameObjects.size(); i++)
			{
				// 2D Tree
				if (gameObjects[i]->accessComponent<UserData>()->isTree == true)
				{
					alphaBatch.draw(tree_64, &glm::vec4(0.0f, 0.0f, tree_64->getWidth(), tree_64->getHeight()),
						gameObjects[i]->accessComponent<Transform>()->getXPosition() * tileSize2d, gameObjects[i]->accessComponent<Transform>()->getYPosition() * tileSize2d, tree_64->getWidth(),
						tree_64->getHeight(), tree_64->getWidth() / 2, tree_64->getHeight() / 2, gameObjects[i]->accessComponent<Transform>()->getRotation(), 1.0f, Renderer::clrWhite, 
						gameObjects[i]->accessComponent<UserData>()->spriteColorA, 0.8f + i*0.000001f);
				}
				// 2D Fireball
				else if (gameObjects[i]->accessComponent<UserData>()->isFireball == true)
				{
					alphaBatch.draw(animPlayer2d.getTexture(), animPlayer2d.getCurrentFrameTexCoords(),
						gameObjects[i]->accessComponent<Transform>()->getXPosition() * tileSize2d, gameObjects[i]->accessComponent<Transform>()->getYPosition() * tileSize2d, 256, 256, 256 / 2, 256 / 2, gameObjects[i]->accessComponent<Transform>()->getRotation(),
						0.25f, Renderer::clrWhite, gameObjects[i]->accessComponent<UserData>()->spriteColorA, 0.8f + i * 0.001f);
				}

				// Hit animation.
				else if (gameObjects[i]->accessComponent<UserData>()->isHitAnimation == true)
				{
					alphaBatch.draw(gameObjects[i]->accessComponent<AnimationPlayer>()->getTexture(), gameObjects[i]->accessComponent<AnimationPlayer>()->getCurrentFrameTexCoords(),
						gameObjects[i]->accessComponent<Transform>()->getXPosition() * tileSize2d, gameObjects[i]->accessComponent<Transform>()->getYPosition() * tileSize2d, 256, 256, 256 / 2, 256 / 2, gameObjects[i]->accessComponent<Transform>()->getRotation(),
						0.3f, Renderer::clrWhite, gameObjects[i]->accessComponent<UserData>()->spriteColorA, 0.8f + i * 0.001f);
				}
				// 2D Furball
				else
				{
					alphaBatch.draw(furball, &glm::vec4(0.0f, 0.0f, furball->getWidth(), furball->getHeight()),
						gameObjects[i]->accessComponent<Transform>()->getXPosition() * tileSize2d, gameObjects[i]->accessComponent<Transform>()->getYPosition() * tileSize2d, furball->getWidth(),
						furball->getHeight(), tileSize2d / 2.0f, tileSize2d / 2.0f, gameObjects[i]->accessComponent<Transform>()->getRotation(), 1.0f,
						Renderer::Color{ gameObjects[i]->accessComponent<UserData>()->spriteColorR, gameObjects[i]->accessComponent<UserData>()->spriteColorG, gameObjects[i]->accessComponent<UserData>()->spriteColorB }, 
						gameObjects[i]->accessComponent<UserData>()->spriteColorA, 0.7f + i*0.000001f);
				}
			}

			//player
			alphaBatch.draw(furball_128, &glm::vec4(0.0f, 0.0f, furball_128->getWidth(), furball_128->getHeight()), player[0] * tileSize2d, player[1] * tileSize2d, furball_128->getWidth(), furball_128->getHeight(), furball_128->getWidth() / 2.0f, furball_128->getHeight() / 2.0f, player[2], 1.0f, Renderer::clrRed, 1.0f, 0.8f);

			//2d floor
			opaqueBatch.draw(floor_16, &glm::vec4(0.0f, 0.0f, mapX * tileSize2d, mapY * tileSize2d), 0.0f, 0.0f, mapX * tileSize2d, mapY * tileSize2d, 0.0f, 0.0f, 0.0f, 1.0f, Renderer::Color{ 0.3, 0.3, 0.4 } *3.0f, 1.0f, 0.0f);
			//---------------
		}

		// Raycast furball sprite.
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
			gameObjects.back()->accessComponent<UserData>()->spriteColorR = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorG = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorB = 1.0f;

			//rigidBody
			gameObjects.back()->accessComponent<RigidBody>()->setCollisionRadius(0.4f);
		}

		// Raycast hit animation.
		void Pseudo3D::createHitFireball(float x, float y, Game::Transform* furbalGameobjectTransform)
		{
			gameObjects.push_back(new GameObject(&alphaBatch));
			gameObjects.back()->addComponent<Transform>();
			gameObjects.back()->addComponent<RigidBody>();
			gameObjects.back()->addComponent<Sprite>();
			gameObjects.back()->addComponent<AnimationPlayer>();
			gameObjects.back()->addComponent<UserData>();
			gameObjects.back()->addComponent<PseudoSpriteDraw>();

			gameObjects.back()->accessComponent<AnimationPlayer>()->setAnimation(animFurballHit);
			gameObjects.back()->accessComponent<AnimationPlayer>()->setLoopEndFrame(36);
			gameObjects.back()->accessComponent<AnimationPlayer>()->loopable(true);
			gameObjects.back()->accessComponent<AnimationPlayer>()->start();

			gameObjects.back()->accessComponent<Transform>()->setXPosition(x);
			gameObjects.back()->accessComponent<Transform>()->setYPosition(y);
			gameObjects.back()->accessComponent<Transform>()->setRotation(0.0f);

			//userdata
			gameObjects.back()->accessComponent<UserData>()->sides = 1;
			gameObjects.back()->accessComponent<UserData>()->transformY = 0.0f;
			gameObjects.back()->accessComponent<UserData>()->animationIndex = 0;
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setTextureBatch(&alphaBatch);
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setRaycastW(raycastW);
			gameObjects.back()->accessComponent<UserData>()->spriteColorR = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorG = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorB = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->isHitAnimation = true;
			gameObjects.back()->accessComponent<UserData>()->depthRandom = 0.000001f;

			gameObjects.back()->accessComponent<UserData>()->hitCoolDown.start();
			gameObjects.back()->accessComponent<UserData>()->cooldownLenght = 1110.0f;

			gameObjects.back()->accessComponent<UserData>()->furbalGameobjectTransform = furbalGameobjectTransform;
		}

		// Raycast tree sprite.
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
			gameObjects.back()->accessComponent<UserData>()->isTree = true;
			gameObjects.back()->accessComponent<UserData>()->depthRandom = randomGenerator.getRandomFloat(-0.000001, 0.000001);
			gameObjects.back()->accessComponent<UserData>()->spriteColorR = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorG = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorB = 1.0f;

			gameObjects.back()->accessComponent<UserData>()->shadow = treeShadow;
			gameObjects.back()->accessComponent<UserData>()->hasShadow = true;
		}

		// Raycast fireball projectile.
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

			// Userdata.
			gameObjects.back()->accessComponent<UserData>()->sides = 8;
			gameObjects.back()->accessComponent<Transform>()->setDepth(1.0f);
			gameObjects.back()->accessComponent<UserData>()->transformY = 0.0f;
			gameObjects.back()->accessComponent<UserData>()->animationIndex = 0;
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setTextureBatch(&alphaBatch);
			gameObjects.back()->accessComponent<PseudoSpriteDraw>()->setRaycastW(raycastW);
			gameObjects.back()->accessComponent<UserData>()->spriteColorR = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorG = 1.0f;
			gameObjects.back()->accessComponent<UserData>()->spriteColorB = 1.0f;

			gameObjects.back()->accessComponent<UserData>()->isFireball = true;

			// RigidBody.
			gameObjects.back()->accessComponent<RigidBody>()->setCollisionRadius(0.2f);
		}

		void Pseudo3D::Projectile::update()
		{
			Transform* t = ownerObject->accessComponent<Transform>();
			t->setXPosition(t->getXPosition() + speed * cosf(t->getRotation() + glm::radians(rng.getRandomFloat(-spread, spread))));
			t->setYPosition(t->getYPosition() + speed * sinf(t->getRotation() + glm::radians(rng.getRandomFloat(-spread, spread))));
		}

		// Deletes killed objects from the gameObjects vector.
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