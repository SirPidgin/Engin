#pragma once
#include <string>

namespace Engin
{
	namespace Resources
	{
		class Resource
		{
			friend class ResourceManager;
		public:
			Resource(const std::string& resourcePath);
			virtual ~Resource();

			std::string& getResourcePath();
			int getReferenceCount();

		protected:
			std::string resourcePath;

			int references;
			void incReferences();
			void decReferences();
		};
	}
}