#include "Engin\Resources\Animation.h"
#include "Engin\Resources\ResourceManager.h"
#include "RapidXml\rapidxml.hpp"

#include <fstream>
#include <sstream>
#include <string>

namespace Engin
{
	namespace Resources
	{
		Animation::Animation(const std::string& resourcePath) : Resource(resourcePath), frameWidth(0), frameHeight(0)
		{
			rapidxml::xml_document<> document;

			std::ifstream file(resourcePath);
			std::stringstream buffer;

			buffer << file.rdbuf();

			file.close();

			std::string content(buffer.str());
			document.parse<0>(&content[0]);

			rapidxml::xml_node<>* root = document.first_node();

			frameWidth = atoi(root->first_attribute("width")->value());
			frameHeight = atoi(root->first_attribute("height")->value());
			texture = Resources::ResourceManager::getInstance().load<Texture>(root->first_attribute("path")->value());

			int framesInARow = texture->getWidth() / frameWidth;

			for (rapidxml::xml_node<>* i = root->first_node("frame"); i; i = i->next_sibling())
			{
				Frame frame;
				frame.duration = atoi(i->first_attribute("duration")->value());

				int index = atoi(i->first_attribute("id")->value());
				frame.texCoords.x = (index % framesInARow) * frameWidth;
				frame.texCoords.y = (index / framesInARow) * frameHeight;
				frame.texCoords.z = frameWidth;
				frame.texCoords.w = frameHeight;

				frames.push_back(frame);
			}

			content.clear();
			buffer.clear();
			document.clear();
		}

		Animation::~Animation()
		{
			frames.clear();
		}
	}
}