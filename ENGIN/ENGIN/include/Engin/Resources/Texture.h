#pragma once
#include "Engin\Resources\Resource.h"

namespace Engin
{
	namespace Resources
	{
		class Texture : public Resource
		{
		public:
			Texture(const std::string& resourcePath);
			~Texture();
		};
	}
}
