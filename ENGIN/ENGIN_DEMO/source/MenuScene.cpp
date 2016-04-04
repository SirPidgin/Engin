#include "MenuScene.h"
#include "Pseudo3D.h"
#include "CameraTestScene.h"
#include "GameObjectTestScene.h"
#include "PhysicsDemo.h"
#include <iostream>

namespace Engin
{
	namespace Game
	{
		MenuScene::MenuScene(Engin* engine) : engine(engine), camera(createGuiCamera()), selected(FURBALLSTEIN), useGamePad(false)
		{
			std::cout << "MENU SCENE IS A GO " << std::endl;

			if (engine->gamepadInput->getNumGamepads() > 0)
			{
				useGamePad = true;
			}

			camera->initCamera(0.0f, 0.0f, 1600.0f, 800.0f, 0.0f, 0.0f, 800.0f, 400.0f);

			// Load menu texture.
			menuTexture = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/menuscreen.png");
			shader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/texture_shader");
			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(100);

			optionFurballsteinCreator.createTextTexture(font, "FURBALLSTEIN 3D", 255, 255, 255);
			optionFurballstein = optionFurballsteinCreator.getTexture();

			optionOriginalCreator.createTextTexture(font, "ORIGINAL DEMO", 255, 255, 255);
			optionOriginal = optionOriginalCreator.getTexture();

			optionParticleCreator.createTextTexture(font, "PARTICLES", 255, 255, 255);
			optionParticle = optionParticleCreator.getTexture();

			optionPhysicsCreator.createTextTexture(font, "PHYSICS DEMO", 255, 255, 255);
			optionPhysics = optionPhysicsCreator.getTexture();

			optionQuitCreator.createTextTexture(font, "QUIT", 255, 255, 255);
			optionQuit = optionQuitCreator.getTexture();

			guiBatch.setShader(shader);
		}

		MenuScene::~MenuScene()
		{
			std::cout << "MENU SCENE IS A GO " << std::endl;

			// Unload resources.
			Resources::ResourceManager::getInstance().unload(menuTexture->getResourcePath());
			Resources::ResourceManager::getInstance().unload(shader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(font->getResourcePath());
		}

		void MenuScene::update(GLfloat step)
		{
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_RETURN) || 
				engine->keyboardInput->keyWasPressed(HID::KEYBOARD_SPACE) || 
				(useGamePad && engine->gamepadInput->buttonWasPressed(HID::GAMEPAD_BUTTON_A, 0)))
			{
				switch (selected)
				{
					case FURBALLSTEIN:
					{
						engine->getSceneManager().push(new Pseudo3D(engine));
						break;
					}
					case ORIGINAL:
					{
						engine->getSceneManager().push(new CameraTestScene(engine));
						break;
					}
					case PARTICLE:
					{
						engine->getSceneManager().push(new GameObjectTestScene(engine));
						break;
					}
					case PHYSICS:
					{
						engine->getSceneManager().push(new PhysicsDemo(engine));
						break;
					}
					case QUIT:
					{
						engine->stop();
						break;
					}
					default:
					{
						break;
					}
				}
			}
			else if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_ESCAPE) ||
				(useGamePad && engine->gamepadInput->buttonWasPressed(HID::GAMEPAD_BUTTON_B, 0)))
			{
				engine->stop();
			}
			else if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_UP) || 
				engine->keyboardInput->keyWasPressed(HID::KEYBOARD_W) ||
				(useGamePad && engine->gamepadInput->buttonWasPressed(HID::GAMEPAD_BUTTON_DPAD_UP, 0)))
			{
				switch (selected)
				{
					case FURBALLSTEIN:
					{
						selected = QUIT;
						break;
					}
					case ORIGINAL:
					{
						selected = FURBALLSTEIN;
						break;
					}
					case PARTICLE:
					{
						selected = ORIGINAL;
						break;
					}
					case PHYSICS:
					{
						selected = PARTICLE;
						break;
					}
					case QUIT:
					{
						selected = PHYSICS;
						break;
					}
					default:
					{
						break;
					}
				}
			}
			else if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_DOWN) ||
				engine->keyboardInput->keyWasPressed(HID::KEYBOARD_S) ||
				(useGamePad && engine->gamepadInput->buttonWasPressed(HID::GAMEPAD_BUTTON_DPAD_DOWN, 0)))
			{
				switch (selected)
				{
					case FURBALLSTEIN:
					{
						selected = ORIGINAL;
						break;
					}
					case ORIGINAL:
					{
						selected = PARTICLE;
						break;
					}
					case PARTICLE:
					{
						selected = PHYSICS;
						break;
					}
					case PHYSICS:
					{
						selected = QUIT;
						break;
					}
					case QUIT:
					{
						selected = FURBALLSTEIN;
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}

		void MenuScene::interpolate(GLfloat alpha)
		{

		}

		void MenuScene::draw()
		{
			guiBatch.draw(menuTexture, 0.0f, 0.0f);

			if (selected == FURBALLSTEIN)
			{
				guiBatch.draw(optionFurballstein, 350.0f, 180, optionFurballstein->getWidth(), optionFurballstein->getHeight(), Renderer::clrRed, 1.0f, 0.1f);
				guiBatch.draw(optionOriginal, 350.0f, 60.0f, optionOriginal->getWidth(), optionOriginal->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionParticle, 350.0f, -60.0f, optionParticle->getWidth(), optionParticle->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionPhysics, 350.0f, -180.0f, optionPhysics->getWidth(), optionPhysics->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionQuit, 350.0f, -300.0f, optionQuit->getWidth(), optionQuit->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
			}
			else if (selected == ORIGINAL)
			{
				guiBatch.draw(optionFurballstein, 350.0f, 180, optionFurballstein->getWidth(), optionFurballstein->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionOriginal, 350.0f, 60.0f, optionOriginal->getWidth(), optionOriginal->getHeight(), Renderer::clrRed, 1.0f, 0.1f);
				guiBatch.draw(optionParticle, 350.0f, -60.0f, optionParticle->getWidth(), optionParticle->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionPhysics, 350.0f, -180.0f, optionPhysics->getWidth(), optionPhysics->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionQuit, 350.0f, -300.0f, optionQuit->getWidth(), optionQuit->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
			}
			else if (selected == PARTICLE)
			{
				guiBatch.draw(optionFurballstein, 350.0f, 180, optionFurballstein->getWidth(), optionFurballstein->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionOriginal, 350.0f, 60.0f, optionOriginal->getWidth(), optionOriginal->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionParticle, 350.0f, -60.0f, optionParticle->getWidth(), optionParticle->getHeight(), Renderer::clrRed, 1.0f, 0.1f);
				guiBatch.draw(optionPhysics, 350.0f, -180.0f, optionPhysics->getWidth(), optionPhysics->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionQuit, 350.0f, -300.0f, optionQuit->getWidth(), optionQuit->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
			}
			else if (selected == PHYSICS)
			{
				guiBatch.draw(optionFurballstein, 350.0f, 180, optionFurballstein->getWidth(), optionFurballstein->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionOriginal, 350.0f, 60.0f, optionOriginal->getWidth(), optionOriginal->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionParticle, 350.0f, -60.0f, optionParticle->getWidth(), optionParticle->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionPhysics, 350.0f, -180.0f, optionPhysics->getWidth(), optionPhysics->getHeight(), Renderer::clrRed, 1.0f, 0.1f);
				guiBatch.draw(optionQuit, 350.0f, -300.0f, optionQuit->getWidth(), optionQuit->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
			}
			else if (selected == QUIT)
			{
				guiBatch.draw(optionFurballstein, 350.0f, 180, optionFurballstein->getWidth(), optionFurballstein->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionOriginal, 350.0f, 60.0f, optionOriginal->getWidth(), optionOriginal->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionParticle, 350.0f, -60.0f, optionParticle->getWidth(), optionParticle->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionPhysics, 350.0f, -180.0f, optionPhysics->getWidth(), optionPhysics->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionQuit, 350.0f, -300.0f, optionQuit->getWidth(), optionQuit->getHeight(), Renderer::clrRed, 1.0f, 0.1f);
			}
		}
	}
}