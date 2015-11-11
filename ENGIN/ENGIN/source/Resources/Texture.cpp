#include "Engin\Resources\Texture.h"
#include "LodePNG\lodepng.h"

namespace Engin
{
	namespace Resources
	{
		Texture::Texture(const std::string& resourcePath) : Resource(resourcePath)
		{
			lodepng::load_file(file, resourcePath);
			lodepng::decode(pixels, width, height, file.data(), file.size());

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture::Texture(const GLuint id, const GLuint w, const GLuint h) : Resource("__INTERNAL__")
		{
			textureID = id;
			width = w;
			height = h;
		}

		Texture::~Texture()
		{
			glDeleteTextures(1, &textureID);
		}

		void Texture::changeParameters(GLenum parameterName, GLint parameter)
		{
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, parameterName, parameter);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Texture::bind(GLenum textureunit)
		{
			glActiveTexture(textureunit);
			glBindTexture(GL_TEXTURE_2D, textureID);
		}

		void Texture::unbind()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}