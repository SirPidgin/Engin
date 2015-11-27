#include "Engin\Game\AnimationPlayer.h"

namespace Engin
{
	namespace Game
	{
		AnimationPlayer::AnimationPlayer(GameObject* o) : Component(o), loop(false), currentFrame(0.0f), loopStartFrame(0), loopEndFrame(0)
		{

		}

		AnimationPlayer::AnimationPlayer() : Component(nullptr), loop(false), currentFrame(0.0f), loopStartFrame(0), loopEndFrame(0)
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
					if (currentFrame < loopStartFrame)
					{
						currentFrame = loopStartFrame;
					}
					if (++currentFrame >= loopEndFrame || currentFrame >= animation->getNumberOfFrames())
					{
						stop();

						if (loop)
						{
							start();
						}
					}
					else
					{
						start();
					}
				}
			}
		}

		void AnimationPlayer::setAnimation(Resources::Animation* animation)
		{
			if (animation)
			{
				this->animation = animation;
				loopEndFrame = animation->getNumberOfFrames();
			}
		}
	}
}
