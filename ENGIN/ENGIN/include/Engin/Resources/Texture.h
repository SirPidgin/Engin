#pragma once
#include "Engin\Resources\Resource.h"
#include "GL\glew.h"
#include <vector>

namespace Engin
{
	namespace Resources
	{
		class Texture : public Resource
		{
		public:
			Texture(const std::string& resourcePath);
			Texture(const GLuint id, const GLuint w, const GLuint h);
			~Texture();

			void changeParameters(GLenum parameterName, GLint parameter);
			void bind(GLenum textureUnit);
			void unbind();

			GLuint getWidth()
			{ 
				return width;
			}

			GLuint getHeight() 
			{
				return height;
			}

		private:
			GLuint textureID;
			GLuint width;
			GLuint height;

			std::vector<unsigned char> file;
			std::vector<unsigned char> pixels;
		};
	}
}