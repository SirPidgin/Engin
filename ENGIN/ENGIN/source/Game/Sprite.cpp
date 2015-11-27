#include "Engin\Game\Sprite.h"
#include "Engin\Renderer\Color.h"
#include "Engin\Game\GameObject.h"


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
			textureBatch->draw(currentTexture, nullptr, ownerObject->accessComponent<Transform>()->getXPosition(), ownerObject->accessComponent<Transform>()->getYPosition(), currentTexture->getWidth(),
				currentTexture->getHeight(), ownerObject->accessComponent<Transform>()->getRotation(), ownerObject->accessComponent<Transform>()->getScale(), Renderer::clrWhite, 1.0f, ownerObject->accessComponent<Transform>()->getDepth());
		}

		void Sprite::setCurrentSprite(Resources::Texture* texture)
		{
			currentTexture = texture;
		}
	}
}