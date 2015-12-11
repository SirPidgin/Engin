#include "Engin\Game\Scene.h"

namespace Engin
{
	namespace Game
	{
		Scene::Scene()
		{
			alphaBatch.setSortMode(Renderer::TextureSortMode::FrontToBack);
			guiBatch.setSortMode(Renderer::TextureSortMode::FrontToBack);
		}

		Scene::~Scene()
		{
			for (auto camera : worldCameras)
			{
				delete camera;
			}

			for (auto camera : guiCameras)
			{
				delete camera;
			}

			worldCameras.clear();
			guiCameras.clear();
		}

		Renderer::Camera* Scene::createWorldCamera()
		{
			worldCameras.push_back(new Renderer::Camera());
			return worldCameras.back();
		}

		Renderer::Camera* Scene::createGuiCamera()
		{
			guiCameras.push_back(new Renderer::Camera());
			return guiCameras.back();
		}

		void Scene::flushBatches()
		{
			for (auto camera : worldCameras)
			{
				camera->activateViewPort();
				opaqueBatch.flush(*camera);
				alphaBatch.flush(*camera);
			}

			opaqueBatch.clear();
			alphaBatch.clear();

			for (auto camera : guiCameras)
			{
				camera->activateViewPort();
				guiBatch.flush(*camera);
			}

			guiBatch.clear();
		}

		void Scene::begin()
		{
			opaqueBatch.begin();
			alphaBatch.begin();
			guiBatch.begin();
		}

		void Scene::end()
		{
			opaqueBatch.end();
			alphaBatch.end();
			guiBatch.end();
		}
	}
}