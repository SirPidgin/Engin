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
			// Loads the animation data.
			rapidxml::xml_document<> document; // The animation data document.

			// Open the file, read the data to the buffer, close the file, create a string from the data and parse it.
			std::ifstream file(resourcePath);
			std::stringstream buffer;

			buffer << file.rdbuf();

			file.close();

			std::string content(buffer.str());
			document.parse<0>(&content[0]);

			rapidxml::xml_node<>* root = document.first_node(); // Get the root node of xml document.

			// Read frame width and height and load the texture sheet.
			frameWidth = atoi(root->first_attribute("width")->value());
			frameHeight = atoi(root->first_attribute("height")->value());
			texture = Resources::ResourceManager::getInstance().load<Texture>(root->first_attribute("path")->value());

			int framesInARow = texture->getWidth() / frameWidth; // Calculate the number of frames in one a row in the texture sheet.

			// Read the frames.
			for (rapidxml::xml_node<>* i = root->first_node("frame"); i; i = i->next_sibling())
			{
				Frame frame;
				frame.duration = float(atoi(i->first_attribute("duration")->value())); // Read the duration of frame.

				// Calculate the index and the texture coordinates of the frame.
				int index = atoi(i->first_attribute("id")->value());
				frame.texCoords.x = float((index % framesInARow) * frameWidth);
				frame.texCoords.y = float((index / framesInARow) * frameHeight);
				frame.texCoords.z = frameWidth;
				frame.texCoords.w = frameHeight;

				frames.push_back(frame);
			}

			// Clear data.
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