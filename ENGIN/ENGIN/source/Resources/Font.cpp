#include "Engin\Resources\Font.h"
#include <iostream>

namespace Engin
{
	namespace Resources
	{
		Font::Font(const std::string& resourcePath) : Resource(resourcePath)
		{
			ptsize = 18;

			this->resourcePath = resourcePath;
				
			font = TTF_OpenFont(resourcePath.c_str(), ptsize);
			if (font == NULL) 
			{
				std::cout << "Couldn't load %d pt font from %s: %s\n"<<
					ptsize << resourcePath << std::endl;
			}
		}

		GLuint Font::getPtSize() 
		{
			return ptsize;
		}
		void Font::setPtSize(const GLuint ptsize) 
		{
			if (this->ptsize != ptsize) {
				this->ptsize = ptsize;

				font = TTF_OpenFont(this->resourcePath.c_str(), ptsize);
			}
		}

		TTF_Font* Font::getFont()
		{
			return font;
		}
	}
}