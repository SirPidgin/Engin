#include "TitleScene.h"
#include "MenuScene.h"
#include "Engin\Resources\ResourceManager.h"
#include <iostream>

namespace Engin
{
	namespace Game
	{
		TitleScene::TitleScene(Engin* engine) : engine(engine), camera(createGuiCamera())
		{
			std::cout << "TITLE SCENE IS A GO " << std::endl;

			camera->initCamera(0.0f, 0.0f, 1600.0f, 800.0f, 0.0f, 0.0f, 800.0f, 400.0f);

			// Load title texture.
			textureShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/texture_shader");
			titleTexture = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/titlescreen.png");

			guiBatch.setShader(textureShader);
		}

		TitleScene::~TitleScene()
		{
			std::cout << "TITLE SCENE IS A DIE " << std::endl;

			// Unload resources.
			Resources::ResourceManager::getInstance().unload(shader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(textureShader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(alphaShader->getResourcePath());
			Resources::ResourceManager::getInstance().unload(furball->getResourcePath());
			Resources::ResourceManager::getInstance().unload(furball_128->getResourcePath());
			Resources::ResourceManager::getInstance().unload(tree_64->getResourcePath());
			Resources::ResourceManager::getInstance().unload(mapSheet_64->getResourcePath());
			Resources::ResourceManager::getInstance().unload(mapSheet_256->getResourcePath());
			Resources::ResourceManager::getInstance().unload(floor_16->getResourcePath());
			Resources::ResourceManager::getInstance().unload(floor_800->getResourcePath());
			Resources::ResourceManager::getInstance().unload(font->getResourcePath());
			Resources::ResourceManager::getInstance().unload(animFurball360->getResourcePath());
			Resources::ResourceManager::getInstance().unload(furballShadow->getResourcePath());
			Resources::ResourceManager::getInstance().unload(animFireball360->getResourcePath());
			Resources::ResourceManager::getInstance().unload(animTree360->getResourcePath());
			Resources::ResourceManager::getInstance().unload(treeShadow->getResourcePath());
			Resources::ResourceManager::getInstance().unload(animFurballHit->getResourcePath());
			Resources::ResourceManager::getInstance().unload(titleTexture->getResourcePath());
		}

		void TitleScene::update(GLfloat step)
		{
			loadResources();

			engine->getSceneManager().push(new MenuScene(engine));
		}

		void TitleScene::interpolate(GLfloat alpha)
		{

		}

		void TitleScene::draw()
		{
			guiBatch.draw(titleTexture, 0.0f, 0.0f);
		}

		void TitleScene::loadResources()
		{
			// Load resources.
			std::cout << "Started loading resources" << std::endl;

			shader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/shader");
			alphaShader = Resources::ResourceManager::getInstance().load<Resources::ShaderProgram>("resources/shaders/alpha_shader");
			furball = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_upside_64.png");
			furball_128 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_upside_128.png");
			mapSheet_64 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/map_sheet_64.png");
			mapSheet_256 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/map_sheet_256_shadows.png");
			floor_16 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/roof.png");
			floor_800 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/floor.png");
			animFurball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/furball360_40.xml");
			animFireball360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/fireball360_8.xml");
			furballShadow = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/furball_shadow_256.png");
			animTree360 = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/tree360_40.xml");
			treeShadow = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/tree_shadow_512.png");
			tree_64 = Resources::ResourceManager::getInstance().load<Resources::Texture>("resources/tree_up_64.png");
			animFurballHit = Resources::ResourceManager::getInstance().load<Resources::Animation>("resources/animations/fireball_hit_36.xml");
			font = Resources::ResourceManager::getInstance().load<Resources::Font>("resources/arial.ttf");

			std::cout << "Resources loaded" << std::endl;

			Resources::ResourceManager::getInstance().printResources();
		}
	}
}