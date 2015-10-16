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

			void update(GLfloat deltaTime);
			void draw();

			void push(Scene* scene);
			void pop();
			void change(Scene* scene);
		private:
			std::vector<Scene*> scenes;
		};
	}
}