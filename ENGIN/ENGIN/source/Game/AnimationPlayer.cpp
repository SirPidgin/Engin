#include "Engin\Game\AnimationPlayer.h"

namespace Engin
{
	namespace Game
	{
		AnimationPlayer::AnimationPlayer(GameObject* o) : 
			Component(o), 
			loop(false), 
			currentFrame(0), 
			loopStartFrame(0), 
			loopEndFrame(0)
		{

		}

		AnimationPlayer::AnimationPlayer() : 
			Component(nullptr), 
			loop(false), 
			currentFrame(0), 
			loopStartFrame(0), 
			loopEndFrame(0)
		{

		}

		AnimationPlayer::~AnimationPlayer()
		{
			animation = nullptr;
		}

		// Updates the animation.
		void AnimationPlayer::update()
		{
			// Update only when timer is started and not paused.
			if (!timer.isPaused() && timer.isStarted())
			{
				// Compare local time to the duration of current frame.
				if (timer.getLocalTime() > animation->getFrame(currentFrame).duration)
				{
					// Petrin hax?
					if (currentFrame < loopStartFrame)
					{
						currentFrame = loopStartFrame;
					}

					// Advance current frame and check if we have reached the end of the animation.
					if (++currentFrame >= loopEndFrame || currentFrame >= animation->getNumberOfFrames())
					{
						stop(); // Stop the animation.

						// Continue if the animation is set to loop.
						if (loop)
						{
							start(); // Restarts the timer.
						}
					}
					else
					{
						start(); // Restarts the timer.
					}
				}
			}
		}

		// Sets the animation used by the player.
		void AnimationPlayer::setAnimation(Resources::Animation* animation)
		{
			if (animation)
			{
				this->animation = animation;
				loopEndFrame = animation->getNumberOfFrames(); // By default we set the end frame to be the last frame of the animation.
			}
		}
	}
}
