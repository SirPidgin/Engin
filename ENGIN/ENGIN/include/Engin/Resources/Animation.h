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
		// Represents a single frame in the animation.

		struct Frame
		{
			float duration;
			glm::vec4 texCoords;
		};

		// Animation is a resource used by an animation player. It consist of
		// a texture sheet (all frames in a single texture) and an animation
		// data file (in .xml format).

		class Animation : public Resource
		{
		public:
			Animation(const std::string& resourcePath);
			~Animation();

			size_t getNumberOfFrames() const
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

			int getFrameWidth() const
			{
				return frameWidth;
			}

			int getFrameHeight() const
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