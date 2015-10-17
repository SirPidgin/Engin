#pragma once
#include <GL\glew.h>

namespace Engin
{
	namespace Game
	{
		class Scene
		{
		public:
			Scene(){};
			virtual ~Scene(){};
			
			virtual void update(GLfloat deltaTime) = 0;
			virtual void draw() = 0;
		private:
			GLfloat accumulator;
			GLfloat step;
		};
	}
}