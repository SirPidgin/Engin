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
			textureBatch->draw(currentTexture, NULL, getXPosition(), getYPosition(), currentTexture->getWidth(), 
				currentTexture->getHeight(), getRotation(), getScale(), Renderer::clrWhite);
		}

		void Sprite::setCurrentSprite(Resources::Texture* texture)
		{
			currentTexture = texture;
		}
	}
}