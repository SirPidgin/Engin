#pragma once

#include <GL/glew.h>
#include <string>
#include "Engin\Resources\Resource.h"

namespace Engin
{
	namespace Resources
	{
		class ShaderProgram : public Resource
		{
		public:
			ShaderProgram(const std::string &resourcePath);
			~ShaderProgram();

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