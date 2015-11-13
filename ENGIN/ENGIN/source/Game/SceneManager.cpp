#include "Engin\Game\SceneManager.h"
#include "Engin\Engin.h"

namespace Engin
{
	namespace Game
	{
		SceneManager::SceneManager()
		{
		}

		SceneManager::~SceneManager()
		{
			for (auto& scene : scenes)
			{
				delete scene;
			}

			scenes.clear();
		}

		void SceneManager::update(GLfloat step)
		{
			scenes.back()->update(step);
		}

		void SceneManager::interpolate(GLfloat alpha)
		{
			scenes.back()->interpolate(alpha);
		}

		void SceneManager::draw()
		{
			scenes.back()->begin();
			scenes.back()->draw();
			scenes.back()->end();
			scenes.back()->flushBatches();
		}

		void SceneManager::push(Scene* scene)
		{
			scenes.push_back(scene);
		}

		void SceneManager::pop()
		{
			delete scenes.back();
			scenes.pop_back();
		}

		void SceneManager::change(Scene* scene)
		{
			if (!scenes.empty())
			{
				delete scenes.back();
				scenes.pop_back();
			}

			scenes.push_back(scene);
		}
	}
}