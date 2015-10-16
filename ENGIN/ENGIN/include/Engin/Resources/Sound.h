#pragma once
#include "Engin\Resources\Resource.h"

namespace Engin
{
	namespace Resources
	{
		class Sound : public Resource
		{
		public:
			Sound(const std::string& resourcePath);
			~Sound();
		};
	}
}