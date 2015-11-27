#pragma once
#include "Engin\Resources\Resource.h"
#include "Engin\Resources\Texture.h"

#include <glm\glm.hpp>
#include <vector>
#include <cassert>

namespace Engin
{
	namespace Resources
	{
		struct Frame
		{
			float duration;
			glm::vec4 texCoords;
		};

		class Animation : public Resource
		{
		public:
			Animation(const std::string& resourcePath);
			~Animation();

			size_t getNumberOfFrames()
			{
				return frames.size();
			}
			
			Texture* getTexture() const
			{
				return texture;
			}

			const Frame& getFrame(size_t index) const
			{
				assert(index < frames.size());

				return frames[index];
			}

			int getFrameWidth()
			{
				return frameWidth;
			}

			int getFrameHeight()
			{
				return frameHeight;
			}

			glm::vec4* getTexCoords(size_t index)
			{
				assert(index < frames.size());

				return &frames[index].texCoords;
			}

		private:
			Texture* texture;
			std::vector<Frame> frames;
			int frameWidth;
			int frameHeight;
		};


	}
}