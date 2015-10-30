#pragma once

#include <GL/glew.h>

namespace Engin
{
	namespace Resources
	{
		class Shader
		{
		public:
			Shader(){};
			void load(const GLchar* vertexPath, const GLchar* fragmentPath);
			~Shader();

			void bind();
			void unbind();
			GLuint getProgram() { return program; }
		private:
			GLuint program;
		};
	}
}