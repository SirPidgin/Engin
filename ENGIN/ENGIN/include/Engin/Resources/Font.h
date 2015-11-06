#pragma once
#include "Resource.h"
namespace Engin
{
	namespace Resources
	{
		class Font : public Resource
		{
		public:
			Font(const std::string& resourcePath);
			~Font();
		};
	}
}
