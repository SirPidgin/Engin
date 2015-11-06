#include "Engin\Game\Sprite.h"


namespace Engin
{
	namespace Game
	{
		Sprite::Sprite(Renderer::TextureBatch* textureBatch) : currentTexture(nullptr), textureBatch(textureBatch)
		{
		}
		Sprite::~Sprite()
		{
		}


		void Sprite::draw()
		{
			//IN PROGRESS...
			textureBatch->begin();
			textureBatch->draw(currentTexture, getXPosition(), getYPosition(), 1.0f, 1.0f);
			textureBatch->end();
		}

		void Sprite::setCurrentSprite(Resources::Texture* texture)
		{
			currentTexture = texture;
		}
	}
}