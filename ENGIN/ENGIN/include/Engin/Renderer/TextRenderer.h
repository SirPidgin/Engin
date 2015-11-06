#pragma once
#include "Engin\Resources\Font.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Resources\ResourceManager.h"

namespace Engin
{
	namespace Renderer
	{
		class TextRenderer
		{
		public:
			//Creates an opengl texture
			TextRenderer(){};
			~TextRenderer(){};
			void createTextTexture(Resources::Font* font, GLint textureSizeX, GLint textureSizeY, std::string message, GLint colorR, GLint colorG, GLint colorB);
			Resources::Texture* getTexture();

		private:
			static int power_of_two(int input)
			{
				int value = 1;

				while (value < input) {
					value <<= 1;
				}
				return value;
			}

			GLuint texture;
			GLuint pointSize;
			GLint w, h;
			SDL_Surface *image;
			SDL_Rect area;
			Uint8  saved_alpha;
			SDL_BlendMode saved_mode;
			void SDLLoadTexture();
			
			SDL_Surface *surface;
			Resources::Texture* textureResource;

			GLfloat texCoord[4];
		};
	}
}