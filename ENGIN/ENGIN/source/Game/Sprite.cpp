#include "Engin\Game\Sprite.h"
#include "Engin\Renderer\Color.h"
#include "Engin\Game\GameObject.h"
#include "Engin\Game\AnimationPlayer.h"


namespace Engin
{
	namespace Game
	{
		Sprite::Sprite(GameObject* o) : Component(o), currentTexture(nullptr)
		{
			this->textureBatch = ownerObject->getTextureBatch();
		}

		Sprite::~Sprite()
		{

		}

		void Sprite::draw()
		{
			AnimationPlayer* animationPlayer = ownerObject->accessComponent<AnimationPlayer>();
			Transform* transform = ownerObject->accessComponent<Transform>();
			
			if (animationPlayer)
			{
				textureBatch->draw(animationPlayer->getTexture(), animationPlayer->getCurrentFrameTexCoords(), 
					transform->getXPosition(), transform->getYPosition(),
					animationPlayer->getFrameWidth(), animationPlayer->getFrameHeight(),
					animationPlayer->getFrameWidth() / 2.0f, animationPlayer->getFrameHeight() / 2.0f,
					transform->getRotation(), transform->getScale(),
					Renderer::clrWhite, 1.0f, transform->getDepth());
			}
			else
			{
				textureBatch->draw(currentTexture, nullptr, 
					transform->getXPosition(), transform->getYPosition(),
					currentTexture->getWidth(), currentTexture->getHeight(), 
					currentTexture->getWidth() / 2.0f, currentTexture->getHeight() / 2.0f,
					transform->getRotation(), transform->getScale(),
					Renderer::clrWhite, 1.0f, transform->getDepth());
			}
		}

		void Sprite::setCurrentSprite(Resources::Texture* texture)
		{
			currentTexture = texture;
		}
	}
}