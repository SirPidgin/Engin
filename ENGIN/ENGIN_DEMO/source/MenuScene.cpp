#include "MenuScene.h"
#include "Pseudo3D.h"
#include "CameraTestScene.h"
#include <iostream>

namespace Engin
{
	namespace Game
	{
		MenuScene::MenuScene(Engin* engine) : engine(engine), camera(createGuiCamera()), selected(FURBALLSTEIN)
		{
			std::cout << "MENU SCENE IS A GO " << std::endl;

			camera->initCamera(0.0f, 0.0f, 1600.0f, 800.0f, 0.0f, 0.0f, 800.0f, 400.0f);

			// Load menu texture.
			menuTexture = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/menuscreen.png");
			shader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/texture_shader");
			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");
			font->setPtSize(100);

			optionFurballsteinCreator.createTextTexture(font, "FURBALLSTEIN", 255.0f, 255.0f, 255.0f);
			optionFurballstein = optionFurballsteinCreator.getTexture();

			optionOriginalCreator.createTextTexture(font, "ORIGINAL DEMO", 255.0f, 255.0f, 255.0f);
			optionOriginal = optionOriginalCreator.getTexture();

			optionQuitCreator.createTextTexture(font, "QUIT", 255.0f, 255.0f, 255.0f);
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
			if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_RETURN))
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
			else if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_ESCAPE))
			{
				engine->stop();
			}
			else if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_UP))
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
					case QUIT:
					{
						selected = ORIGINAL;
						break;
					}
					default:
					{
						break;
					}
				}
			}
			else if (engine->keyboardInput->keyWasPressed(HID::KEYBOARD_DOWN))
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
				guiBatch.draw(optionFurballstein, 0.0f, 120.0f, optionFurballstein->getWidth(), optionFurballstein->getHeight(), Renderer::clrRed,  1.0f, 0.1f);
				guiBatch.draw(optionOriginal, 0.0f, 0.0f, optionOriginal->getWidth(), optionOriginal->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionQuit, 0.0f, -120.0f, optionQuit->getWidth(), optionQuit->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
			}
			else if (selected == ORIGINAL)
			{
				guiBatch.draw(optionFurballstein, 0.0f, 120.0f, optionFurballstein->getWidth(), optionFurballstein->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionOriginal, 0.0f, 0.0f, optionOriginal->getWidth(), optionOriginal->getHeight(), Renderer::clrRed, 1.0f, 0.1f);
				guiBatch.draw(optionQuit, 0.0f, -120.0f, optionQuit->getWidth(), optionQuit->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
			}
			else if (selected == QUIT)
			{
				guiBatch.draw(optionFurballstein, 0.0f, 120.0f, optionFurballstein->getWidth(), optionFurballstein->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionOriginal, 0.0f, 0.0f, optionOriginal->getWidth(), optionOriginal->getHeight(), Renderer::clrBlack, 1.0f, 0.1f);
				guiBatch.draw(optionQuit, 0.0f, -120.0f, optionQuit->getWidth(), optionQuit->getHeight(), Renderer::clrRed, 1.0f, 0.1f);
			}
		}
	}
}