#include "PhysicsDemo.h"

#include <iostream>
#include <Engin\Renderer\Window.h>
#include <Engin\Renderer\Color.h>
#include <glm\gtc\type_ptr.hpp>

#include <Engin\Game\Transform.h>

namespace Engin
{
	namespace Game
	{
		PhysicsDemo::PhysicsDemo(Engin* engine) :
			engine(engine),
			camera(createWorldCamera()),
			shader(nullptr),
			tile(nullptr),
			alpha(0),
			useGamePad(false),
			maxObjects(0),
			firstTime(true),
			secondTime(true)
		{
			camera->initCamera(0.0f, 0.0f, static_cast<GLfloat>(engine->getWindow().getWindowWidth()), static_cast<GLfloat>(engine->getWindow().getWindowHeight()), 500.0f, 500.0f, engine->getWindow().getWindowWidth() / 2.0f, engine->getWindow().getWindowHeight() / 2.0f);
			engine->mouseInput->enableRelativeMousPosition();

			if (engine->gamepadInput->getNumGamepads() > 0)
			{
				useGamePad = true;
			}

			std::cout << "Physics Demo SCENE IS A GO" << std::endl;

			shader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/shader");
			textureShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/texture_shader");
			alphaShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/alpha_shader");
			batch.init(shader, 4096);
			opaqueBatch.setShader(textureShader);
			alphaBatch.setShader(alphaShader);
			guiBatch.setShader(alphaShader);

			tile = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/red_tile_128.png");
			tile2 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/green_tile_128.png");

			physicsWorld = new PTPhysicsWorld(128.0f);
			physicsWorld->setGravity(glm::vec2(0, 0));

			fireTimer = new Core::Timer();
			fireTimer->start();
		}

		PhysicsDemo::~PhysicsDemo()
		{
			std::cout << "Physics Demo SCENE IS A DIE" << std::endl;

			engine->mouseInput->disableRelativeMousPosition();

			// Unload resources.
			Resources::ResourceManager::getInstance().unload(tile->getResourcePath());
			Resources::ResourceManager::getInstance().unload(shader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(textureShader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(alphaShader->getResourcePath());
		}

		void PhysicsDemo::update(GLfloat step)
		{
			// Back to menu.
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_ESCAPE) ||
				(useGamePad && engine->gamepadInput->buttonWasPressed(HID::GAMEPAD_BUTTON_B, 0)))
			{
				engine->getSceneManager().pop();
			}

			cameraMovement(step);
			fireTile(600.0f, 0.0f, glm::radians(5.0f), glm::vec2(-100.0f, 0.0f));
			fireTile(-300.0f, 0.0f, glm::radians(0.0f), glm::vec2(100.0f, 0.0f));
			
			if (fireTimer->getLocalTime() > 10000)
			{
				fireTimer->start();
			}			

			physicsWorld->update(step);

			// Looping trough gameObjects update
			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->update();

				// Paras debug draw ikinä
				if(gameObjects[i]->accessComponent<PhysicsComponent>()->getBody()->isColliding())
				{
					gameObjects[i]->accessComponent<Sprite>()->setCurrentSprite(tile2);
				}
				if (!gameObjects[i]->accessComponent<PhysicsComponent>()->getBody()->isColliding())
				{
					gameObjects[i]->accessComponent<Sprite>()->setCurrentSprite(tile);
				}
			}
			
			alpha += step;
			
		}

		void PhysicsDemo::interpolate(GLfloat alpha)
		{
		}

		void PhysicsDemo::draw()
		{
			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->draw();
			}
		}

		void PhysicsDemo::createTile(GLfloat x, GLfloat y,GLfloat r)
		{
			gameObjects.push_back(new GameObject(&alphaBatch));
			gameObjects.back()->addComponent<Sprite>();
			gameObjects.back()->addComponent<Transform>();
			gameObjects.back()->accessComponent<Sprite>()->setCurrentSprite(tile);

			gameObjects.back()->accessComponent<Transform>()->setPosition(glm::vec2(x, y));
			gameObjects.back()->accessComponent<Transform>()->setRotation(r);
			physicsWorld->addRigidBody(new PTRigidBody(physicsWorld), glm::vec2(x, y));
			gameObjects.back()->addComponent<PhysicsComponent>();
			gameObjects.back()->accessComponent<PhysicsComponent>()->setBody(physicsWorld->getBodies().back());
			gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setMass(10);
			gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setHalfSize(glm::vec2(64.0f));			
		}

		void PhysicsDemo::cameraMovement(GLfloat step)
		{
			static float zoomByInput = 1.0f;
			static float moveByInputX = 0.0f;
			static float moveByInputY = 0.0f;
			static float moveSpeed = 300.0f;

			if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_UP) ||
				(useGamePad && engine->gamepadInput->buttonIsPressed(HID::GAMEPAD_BUTTON_RIGHTSHOULDER, 0)))
			{
				if (zoomByInput > 0.0f)
					zoomByInput -= 0.034;
			}
			if (engine->mouseInput->mouseWheelWasMoved(HID::MOUSEWHEEL_DOWN) ||
				(useGamePad && engine->gamepadInput->buttonIsPressed(HID::GAMEPAD_BUTTON_LEFTSHOULDER, 0)))
			{
				zoomByInput += 0.034;
			}

			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_W) ||
				(useGamePad && engine->gamepadInput->buttonIsPressed(HID::GAMEPAD_BUTTON_DPAD_UP, 0)))
			{
				//std::cout << "W" << std::endl;
				moveByInputY += moveSpeed;
			}
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_A) ||
				(useGamePad && engine->gamepadInput->buttonIsPressed(HID::GAMEPAD_BUTTON_DPAD_LEFT, 0)))
			{
				//std::cout << "A" << std::endl;
				moveByInputX -= moveSpeed;
			}
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_S) ||
				(useGamePad && engine->gamepadInput->buttonIsPressed(HID::GAMEPAD_BUTTON_DPAD_DOWN, 0)))
			{
				//std::cout << "S" << std::endl;
				moveByInputY -= moveSpeed;
			}
			if (engine->keyboardInput->keyIsPressed(HID::KEYBOARD_D) ||
				(useGamePad && engine->gamepadInput->buttonIsPressed(HID::GAMEPAD_BUTTON_DPAD_RIGHT, 0)))
			{
				//std::cout << "D" << std::endl;
				moveByInputX += moveSpeed;
			}

			camera->setZoomLevel(zoomByInput); //by input
			camera->setPositionRotationOrigin(moveByInputX*step, moveByInputY*step); //by input
		}

		void PhysicsDemo::fireTile(GLfloat x, GLfloat y, GLfloat r, glm::vec2 veloc)
		{
			if (firstTime || secondTime || fireTimer->getLocalTime() > 10000.0f && maxObjects < 45)
			{
				createTile(x, y, r);
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setVelocity(veloc);
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setRotation(r);
				maxObjects++;
				
				if (!firstTime)
				{
					secondTime = false;
				}

				firstTime = false;
			}
		}
	}
}