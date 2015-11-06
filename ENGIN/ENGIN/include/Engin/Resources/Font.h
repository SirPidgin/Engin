#pragma once
#include "Engin\Resources\Resource.h"
#include <GL\glew.h>
#include <SDL\SDL_ttf.h>

namespace Engin
{
	namespace Resources
	{
		class Font : public Resource
		{
		public:
			Font(const std::string& resourcePath);
			TTF_Font* getFont();
		private:
			GLint ptsize;
			TTF_Font* font;
		};
	}
}