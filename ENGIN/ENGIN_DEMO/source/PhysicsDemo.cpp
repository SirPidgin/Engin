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
			useGamePad(false)
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

			physicsWorld = new PTPhysicsWorld(128.0f);
			physicsWorld->setGravity(glm::vec2(0, -10));
			//// physics bodies
			//for (int i = 0; i < 20; i++)
			//{
			//	physicsWorld->addRigidBody(new PTRigidBody(physicsWorld), glm::vec2(i*128, i*128));
			//}
			//// for drawing
			//for (int i = 0; i < 20; i++)
			//{
			//	createTile(i*128, i*128, i*50);
			//}

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
			fireTile();

			physicsWorld->update(step);

			// Looping trough gameObjects update
			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->update();
			}
			
			alpha += step;
			randomGenerator.setSeed(step);
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

		void PhysicsDemo::fireTile()
		{
			if (fireTimer->getLocalTime() > 2000.0f)
			{
				createTile(0.0f, 200.0f, 0.0f);

				float x = randomGenerator.getRandomFloat(-100.0f, 600.0f);
				float y = randomGenerator.getRandomFloat(40.0f, 600.0f);
				glm::vec2 velocity = glm::vec2(x,y);

				gameObjects.back()->accessComponent<PhysicsComponent>()->getBody()->setVelocity(velocity);

				fireTimer->start();
			}
		}
	}
}