#include "Engin\Game\AnimationPlayer.h"

namespace Engin
{
	namespace Game
	{
		AnimationPlayer::AnimationPlayer() : loop(false), currentFrame(0.0f)
		{

		}
		AnimationPlayer::~AnimationPlayer()
		{
			animation = nullptr;
		}

		void AnimationPlayer::update()
		{
			if (!timer.isPaused() && timer.isStarted())
			{
				if (timer.getLocalTime() > animation->getFrame(currentFrame).duration)
				{
					if (++currentFrame >= animation->getNumberOfFrames())
					{
						stop();

						if (loop)
						{
							start();
						}
					}
				}
			}
		}

		void AnimationPlayer::setAnimation(Resources::Animation* animation)
		{
			if (animation)
			{
				this->animation = animation;
			}
		}
	}
}
