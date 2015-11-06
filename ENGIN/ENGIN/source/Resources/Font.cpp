#include "Engin\Resources\Font.h"
#include <iostream>

namespace Engin
{
	namespace Resources
	{
		Font::Font(const std::string& resourcePath) : Resource(resourcePath)
		{
			std::cout<< "Couldn't initialize TTF: %s\n";

			ptsize = 18; //TODO:: create user changable pointsize
				
			font = TTF_OpenFont(resourcePath.c_str(), ptsize);
			if (font == NULL) 
			{
				std::cout << "Couldn't load %d pt font from %s: %s\n"<<
					ptsize << resourcePath << std::endl;
			}
		}

		TTF_Font* Font::getFont()
		{
			return font;
		}
	}
}