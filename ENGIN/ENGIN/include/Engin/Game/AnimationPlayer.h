#pragma once

#include "Engin\Game\Component.h"
#include "Engin\Resources\Animation.h"
#include "Engin\Core\Timer.h"

namespace Engin
{
	namespace Game
	{
		// Animation player is a gameobject component that manages updating of a given animation.

		class AnimationPlayer : public Component
		{
		public:
			AnimationPlayer(GameObject* o);
			AnimationPlayer();
			~AnimationPlayer();

			void update();
			void setAnimation(Resources::Animation* animation);

			Resources::Texture* getTexture() const
			{ 
				return animation->getTexture(); 
			}

			glm::vec4* getCurrentFrameTexCoords() const
			{ 
				return animation->getTexCoords(currentFrame); 
			}

			int getFrameWidth() const
			{ 
				return animation->getFrameWidth(); 
			}

			int getFrameHeight() const
			{ 
				return animation->getFrameHeight(); 
			}

			void pause() 
			{ 
				timer.pause(); 
			}

			void start() 
			{ 
				timer.start(); 
			}

			void stop() 
			{ 
				timer.stop(); 
				currentFrame = 0 + loopStartFrame; 
			}

			void loopable(bool value) 
			{ 
				loop = value; 
			}

			void setCurrentFrame(int frame)
			{ 
				currentFrame = frame; 

				if (!timer.isPaused())
				{ 
					start(); 
				} 
			}

			int getCurrentFrame() const
			{ 
				return currentFrame; 
			}

			void setLoopStartFrame(int frame)
			{ 
				loopStartFrame = frame;
			}

			void setLoopEndFrame(int frame)
			{ 
				loopEndFrame = frame; 
			}

		private:
			Resources::Animation* animation;
			Core::Timer timer;
			size_t currentFrame;
			size_t loopStartFrame;
			size_t loopEndFrame;

			bool loop;
		};
	}
}
