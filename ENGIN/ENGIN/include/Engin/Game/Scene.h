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
			
			virtual void update(GLfloat step) = 0;
			virtual void interpolate(GLfloat alpha) = 0;
			virtual void draw() = 0;
		private:
		};
	}
}