#pragma once
#include "Engin\Resources\Resource.h"

namespace Engin
{
	namespace Resources
	{
		class Music : public Resource
		{
		public:
			Music(const std::string& resourcePath);
			~Music();
		};
	}
}