#pragma once

#include "Engin\Resources\Animation.h"
#include "Engin\Core\Timer.h"

namespace Engin
{
	namespace Game
	{
		class AnimationPlayer
		{
		public:
			AnimationPlayer();
			~AnimationPlayer();
			void update();

			void setAnimation(Resources::Animation* animation);

			Resources::Texture* getTexture() { return animation->getTexture(); }
			glm::vec4* getCurrentFrameTexCoords() { return animation->getTexCoords(currentFrame); }

			int getFrameWidth() { return animation->getFrameWidth(); }
			int getFrameHeight() { return animation->getFrameHeight(); }

			void pause() { timer.pause(); }
			void start() { timer.start(); }
			void stop() { timer.stop(); currentFrame = 0; }
			void loopable(bool value) { loop = value; }
			void setCurrentFrame(int frame){ currentFrame = frame; };
		private:
			Resources::Animation* animation;
			Core::Timer timer;
			size_t currentFrame;

			bool loop;
		};
	}
}
