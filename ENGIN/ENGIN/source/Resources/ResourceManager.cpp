#include "Engin\Resources\ResourceManager.h"
#include <iostream>

namespace Engin
{
	namespace Resources
	{

		ResourceManager::ResourceManager()
		{
		}


		ResourceManager::~ResourceManager()
		{
			releaseAll();
		}

		bool ResourceManager::unload(const std::string &filename)
		{
			if (filename.empty())
			{
				std::cout << "Filename cannot be null! Error loading resource." << std::endl;
				return false;
			}

			std::unordered_map<std::string, Resource*>::iterator it;
			it = resourceMap.find(filename);

			if (it != resourceMap.end())
			{
				(*it).second->decReferences();

				if ((*it).second->getReferenceCount() == 0)
				{
					delete((*it).second);
					resourceMap.erase(it);
				}
			}
			return true;
		}

		void ResourceManager::releaseAll()
		{
			std::unordered_map<std::string, Resource*>::iterator it;
			it = resourceMap.begin();

			while (it != resourceMap.end())
			{
				delete (*it).second;
				it = resourceMap.erase(it);
			}
		}
	}
}