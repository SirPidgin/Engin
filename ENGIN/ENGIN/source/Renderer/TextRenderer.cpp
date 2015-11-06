#include "Engin\Renderer\TextRenderer.h"

namespace Engin
{
	namespace Renderer
	{
		void TextRenderer::createTextTexture(Resources::Font* font, GLint textureSizeX, GLint textureSizeY, std::string message)
		{
			this->font = font;
			this->w = textureSizeX;
			this->h = textureSizeY;

			this->surface = TTF_RenderText_Blended(this->font->getFont(), message.c_str(), SDL_Color{255});
			SDLLoadTexture();
		}

		Resources::Texture* TextRenderer::getTexture()
		{
			return textureResource;
		}

		void TextRenderer::SDLLoadTexture()
		{
			/* Use the surface width and height expanded to powers of 2 */
			w = power_of_two(surface->w);
			h = power_of_two(surface->h);
			texCoord[0] = 0.0f;         /* Min X */
			texCoord[1] = 0.0f;         /* Min Y */
			texCoord[2] = (GLfloat)surface->w / w;  /* Max X */
			texCoord[3] = (GLfloat)surface->h / h;  /* Max Y */

			image = SDL_CreateRGBSurface(
				SDL_SWSURFACE,
				w, h,
				32,
			#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
				0x000000FF,
				0x0000FF00,
				0x00FF0000,
				0xFF000000
			#else
				0xFF000000,
				0x00FF0000,
				0x0000FF00,
				0x000000FF
			#endif
				);

			/* Save the alpha blending attributes */
			SDL_GetSurfaceAlphaMod(surface, &saved_alpha);
			SDL_SetSurfaceAlphaMod(surface, 0xFF);
			SDL_GetSurfaceBlendMode(surface, &saved_mode);
			SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

			/* Copy the surface into the GL texture image */
			area.x = 0;
			area.y = 0;
			area.w = surface->w;
			area.h = surface->h;
			SDL_BlitSurface(surface, &area, image, &area);

			/* Restore the alpha blending attributes */
			SDL_SetSurfaceAlphaMod(surface, saved_alpha);
			SDL_SetSurfaceBlendMode(surface, saved_mode);

			/* Create an OpenGL texture for the image */
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGBA,
				w, h,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image->pixels);

			this->textureResource->bindTexture(texture);

			SDL_FreeSurface(image); /* No longer needed */
		}
	}
}