#include "Engin\Resources\Resource.h"
#include <iostream>

namespace Engin
{
	namespace Resources
	{
		Resource::Resource(const std::string& resourcePath)
		{
			if (resourcePath.empty())
			{
				std::cout << "Empty filename not allowed!" << std::endl;
			}

			references = 0;
		}

		Resource::~Resource()
		{
		}

		std::string& Resource::getResourcePath()
		{
			return resourcePath;
		}

		int Resource::getReferenceCount()
		{
			return references;
		}

		void Resource::incReferences()
		{
			references++;
		}

		void Resource::decReferences()
		{
			references--;
		}
	}
}