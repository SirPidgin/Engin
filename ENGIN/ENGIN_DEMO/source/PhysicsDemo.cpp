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
			camera->initCamera(0.0f, 0.0f, static_cast<GLfloat>(engine->getWindow().getWindowWidth()), static_cast<GLfloat>(engine->getWindow().getWindowHeight()), 0.0f, 0.0f, engine->getWindow().getWindowWidth() / 2.0f, engine->getWindow().getWindowHeight() / 2.0f);
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

			for (int i = 0; i < 20; i++)
			{
				createTile(i*128, i*128, i*50);
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

			camera->setPositionRotationOrigin(500, 500);

			// Translation test
			for (int i = 0; i < gameObjects.size(); i++)
			{
				float radius = 200;
				float x = gameObjects[i]->accessComponent<Transform>()->getPosition().x + radius * glm::cos(alpha + i) * step;
				float y = gameObjects[i]->accessComponent<Transform>()->getPosition().y + radius * glm::sin(alpha + i) * step;
				gameObjects[i]->accessComponent<Transform>()->setPosition(glm::vec2(x, y));
			}

			// Physics
		

			//

			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i]->update();
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
		}
	}
}