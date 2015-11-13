#pragma once

#include <vector>

#include "Component.h"
#include "Engin\Resources\Texture.h"
#include "Engin\Renderer\TextureBatch.h"


namespace Engin
{
	namespace Game
	{
		class Sprite : public Component
		{
		public:
			Sprite(GameObject* o);
			~Sprite();

			void draw();

			void setCurrentSprite(Resources::Texture* texture);

		private:
			Renderer::TextureBatch* textureBatch;
			Resources::Texture* currentTexture;
		};
	}
}