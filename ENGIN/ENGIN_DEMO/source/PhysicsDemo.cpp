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
			firstTime(true),
			secondTime(true),
			timeToShoot(2000),
			maxObjects(0)
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

			physicsWorld = new PTPhysicsWorld(64.0f);
			physicsWorld->setGravity(glm::vec2(0, 0));

			fireTimer = new Core::Timer();
			fireTimer->start();

			// Pomppulinna =)
			for (int i = -4; i < 4; i++)
			{
				createTile(i * 200.0f, -200.0f, 0.0f);
				//gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setVelocity(glm::vec2(0.0f, 1000.0f));
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->rivetFromMidle();
			}
			// Pomppulinna =)
			for (int i = -4; i < 4; i++)
			{
				createTile(i * 200.0f, 900.0f, 0.0f);
				//gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setVelocity(glm::vec2(0.0f, -1000.0f));
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->rivetFromMidle();
			}
			// Pomppulinna =)
			for (int i = -5; i < 3; i++)
			{
				createTile(800.0f, i*-200.0f, 0.0f);
				//gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setVelocity(glm::vec2(-1000.0f, 0.0f));
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->rivetFromMidle();
			}
			// Pomppulinna =)
			for (int i = -5; i < 3; i++)
			{
				createTile(-1000.0f, i* -200.0f, 0.0f);
				//gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setVelocity(glm::vec2(1000.0f, 0.0f));
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->rivetFromMidle();
			}
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
			fireTile(600.0f, 400.0f, glm::radians(90.0f), glm::vec2(-100.0f, 0.0f));
			fireTile(-600.0f, 400.0f, glm::radians(25.0f), glm::vec2(100.0f, 0.0f));
			
			// TODO: make fireTile into class with own timer.
			if (fireTimer->getLocalTime() > timeToShoot)
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
			deleteDeadObjects();
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
			gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setMass(1000);
			gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setHalfSize(glm::vec2(64.0f));			
		}

		void PhysicsDemo::cameraMovement(GLfloat step)
		{
			static float zoomByInput = 0.5f;
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

		// TODO: make fireTile into class with own timer.
		void PhysicsDemo::fireTile(GLfloat x, GLfloat y, GLfloat r, glm::vec2 veloc)
		{
			if (firstTime || secondTime || fireTimer->getLocalTime() > timeToShoot && maxObjects < 8)
			{
				createTile(x, y, r);
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setVelocity(veloc);
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setRotation(r);
				float rand = randomGenerator.getRandomFloat(-3.0f, 3.0f);
				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setAngularVelocity(rand);
				
				if (!firstTime)
				{
					secondTime = false;
				}

				firstTime = false;
				maxObjects++;
			}
		}

		void PhysicsDemo::deleteDeadObjects()
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