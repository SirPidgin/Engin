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
			Texture() : Resource("__INTERNAL__"), textureID(0), width(0), height(0) {};
			~Texture();

			void changeParameters(GLenum parameterName, GLint parameter);
			void bind(GLenum textureUnit);
			void unbind();

			float getWidth()
			{ 
				return static_cast<float> (width);
			}

			float getHeight() 
			{
				return static_cast<float> (height);
			}

			GLuint getID()
			{
				return textureID;
			}

			void setIDWidthHeight(GLuint id, GLuint w, GLuint h)
			{
				textureID = id;
				width = w;
				height = h;
			}

		private:
			GLuint textureID;
			GLuint width;
			GLuint height;
			std::vector<unsigned char> pixels;
		};
	}
}