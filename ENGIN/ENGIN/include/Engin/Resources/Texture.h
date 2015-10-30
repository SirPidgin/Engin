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
			~Texture();

			void changeParameters(GLenum target, GLenum parameterName, GLint parameter);
			void bindTexture(GLenum textureUnit);

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