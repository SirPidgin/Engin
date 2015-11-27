#pragma once

#include "Engin\Game\Component.h"
#include "Engin\Resources\Animation.h"
#include "Engin\Core\Timer.h"

namespace Engin
{
	namespace Game
	{
		class AnimationPlayer : public Component
		{
		public:
			AnimationPlayer(GameObject* o);
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
			void stop() { timer.stop(); currentFrame = 0 + loopStartFrame; }
			void loopable(bool value) { loop = value; }
			void setCurrentFrame(int frame){ currentFrame = frame; if (!timer.isPaused()){ start(); } }
			int getCurrentFrame(){ return currentFrame; }
			void setLoopStartFrame(int frame){ loopStartFrame = frame; }
			void setLoopEndFrame(int frame){ loopEndFrame = frame; }
		private:
			Resources::Animation* animation;
			Core::Timer timer;
			size_t currentFrame;

			bool loop;

			int loopStartFrame;
			int loopEndFrame;
		};
	}
}
