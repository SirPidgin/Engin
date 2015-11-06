#pragma once

#include <GL/glew.h>

namespace Engin
{
	namespace Resources
	{
		class ShaderProgram
		{
		public:
			ShaderProgram(){};
			~ShaderProgram();

			void load(const GLchar* vertexPath, const GLchar* fragmentPath);
			void bind();
			void unbind();

			GLuint getProgram()
			{
				return program;
			}

		private:
			GLuint program;
		};
	}
}