#pragma once
#include "Engin\Resources\Resource.h"

namespace Engin
{
	namespace Resources
	{
		class Animation : public Resource
		{
		public:
			Animation(const std::string& resourcePath);
			~Animation();
		};
	}
}