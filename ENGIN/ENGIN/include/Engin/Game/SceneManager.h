#pragma once
#include "Engin\Game\Scene.h"
#include <vector>

namespace Engin
{
	namespace Game
	{
		class SceneManager
		{
		public:
			SceneManager();
			~SceneManager();

			// Scenemanager update.
			void update(GLfloat step);

			// Scenemanager interpolate.
			void interpolate(GLfloat alpha);

			// Scenemanager draw.
			void draw();

			// Add new scene for the manager.
			void push(Scene* scene);

			// Remove last scene from the manager.
			void pop();

			// Exchange last scene with new one.
			void change(Scene* scene);
		private:
			std::vector<Scene*> scenes;
		};
	}
}