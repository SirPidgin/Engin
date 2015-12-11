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

			enum SceneAction
			{
				NONE,
				CHANGE,
				PUSH,
				POP
			};

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

			void handleScenes();
		private:
			std::vector<Scene*> scenes;

			SceneAction sceneAction;
			Scene* newScene;
		};
	}
}