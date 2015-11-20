#include "Engin\Renderer\TextRenderer.h"

namespace Engin
{
	namespace Renderer
	{

		TextRenderer::TextRenderer() : surface(nullptr), textureResource(nullptr), image(nullptr)
		{
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			textureResource = new Resources::Texture();
			textureResource->bind(0);
			textureResource->setIDWidthHeight(texture, 0, 0);
			textureResource->unbind();
		};

		TextRenderer::~TextRenderer() 
		{ 
			glDeleteTextures(1, &texture);
			delete textureResource;
			
		};

		void TextRenderer::createTextTexture(Resources::Font* font, std::string message, GLint colorR, GLint colorG, GLint colorB)
		{
			this->surface = TTF_RenderText_Blended(font->getFont(), message.c_str(), SDL_Color{colorR,colorG,colorB});
			SDLLoadTexture();
		}

		Resources::Texture* TextRenderer::getTexture()
		{
			return textureResource;
			SDL_FreeSurface(surface);
		}

		void TextRenderer::SDLLoadTexture()
		{

			/* Use the surface width and height expanded to powers of 2 */
			w = (surface->w);
			h = (surface->h);
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
			textureResource->bind(0);
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGBA,
				w, h,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				image->pixels);

			textureResource->setIDWidthHeight(texture, area.w, area.h);
			textureResource->unbind();

			SDL_FreeSurface(image); /* SDL surface no longer needed */
			SDL_FreeSurface(surface);
		}
	}
}